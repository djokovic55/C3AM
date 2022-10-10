#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/device.h>

#include <linux/io.h> //iowrite ioread
#include <linux/slab.h>//kmalloc kfree
#include <linux/platform_device.h>//platform driver
#include <linux/of.h>//of_match_table
#include <linux/ioport.h>//ioremap

#include <linux/dma-mapping.h>  //dma access
#include <linux/mm.h>  //dma access
#include <linux/interrupt.h>  //interrupt handlers

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR ("g06-2022");
MODULE_DESCRIPTION("Driver for Seam Carving algorithm");
MODULE_ALIAS("custom:seam");
#define DEVICE_NAME "seam"
#define DRIVER_NAME "seam_driver"

#define CACHE_SIZE 25
//adresses for registers
#define COLSIZE     0x00
#define ROWSIZE     0x04
#define START       0x08
#define READY       0x12 

// ------------------------------------------
// DECLARATIONS
// ------------------------------------------

static int seam_open(struct inode *pinode, struct file *pfile);
static int seam_close(struct inode *pinode, struct file *pfile);
static ssize_t seam_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
static ssize_t seam_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
static void hard_cem(void);
static int  __init seam_init(void);
static void __exit seam_exit(void);

int colsize, rowsize, start;
int ready;
int cache[CACHE_SIZE];

struct file_operations my_fops =
{
	.owner   = THIS_MODULE,
	.open    = seam_open,
	.read    = seam_read,
	.write   = seam_write,
	.release = seam_close,
};

static dev_t my_dev_id;
static struct class *my_class;
static struct cdev  *my_cdev;

// ------------------------------------------
// OPEN & CLOSE
// ------------------------------------------

static int seam_open(struct inode *pinode, struct file *pfile)
{
		printk(KERN_INFO "[OPEN] Succesfully opened file\n");
		return 0;
}

static int seam_close(struct inode *pinode, struct file *pfile)
{
		printk(KERN_INFO "[CLOSE] Succesfully closed file\n");
		return 0;
}

// ------------------------------------------
// READ & WRITE
// ------------------------------------------

#define BUFF_SIZE 80
int i = 0;
int pos = 0;
int end_read = 0;

int hard_toggle_row;
int cache_waddr, cache_raddr;

void hard_cem(void)
{
  int a, b, c, min;
  int target_pixel_addr, abc_addr;
  int col = 0;

  if(hard_toggle_row)
    hard_toggle_row = 0;
  else
    hard_toggle_row = 1;

  // Generisanje pocetnih adresa
  if(hard_toggle_row)
  {
      abc_addr = 0;
      target_pixel_addr = colsize;
  } 
  else
  {
      abc_addr = colsize;
      target_pixel_addr = 0;
  }

// Levi granični slucaj
  b = cache[abc_addr];
  min = b;
  c = cache[abc_addr + 1];
      
  if(c < b)
  {
      min = c;
  }

  cache[target_pixel_addr] = cache[target_pixel_addr] + min;
  
  col++;
  abc_addr++;
  target_pixel_addr++;

// Srednji regularni slucaj
  for(col; col < colsize - 1; col++)
  {
    a = b;
    b = c;
    min = b;
    
    c = cache[abc_addr + 1];
    
    if(a < c)
    {
      if(a < b)
        min = a;
    }
    else
    {
      if(c < b)
        min = c;
    }	

    cache[target_pixel_addr] = cache[target_pixel_addr] + min;

    abc_addr++;
    target_pixel_addr++;
  }

// Desni granični slučaj

  a = b;
  b = c;
  min = b;
      
  if(a < b)
  {
      min = a;
  }

  cache[target_pixel_addr] = cache[target_pixel_addr] + min;
}


ssize_t seam_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
  char buf[BUFF_SIZE];
  long int len=0;
  int minor = MINOR(pfile->f_inode->i_rdev);

  printk(KERN_INFO "[READ BEGINS] Seam read entered \n"); 
  // printk(KERN_INFO "i = %d, len = %ld, end_read = %d\n", i, len, end_read);
  if (end_read == 1)
    {
      end_read = 0;
      pos = 0;
      printk(KERN_INFO "\n"); 
      return 0;
    }

  switch (minor)
    {
    case 0: //device seam
      printk(KERN_INFO "[READ ENDS] Succesfully read from seam device.\n");
      //signals here
      
      len = scnprintf(buf, BUFF_SIZE, "[READ] colsize = %d, rowsize = %d, start = %d, ready = %d\n", colsize, rowsize, start, ready);
      if (copy_to_user(buffer, buf, len))
        return -EFAULT;
      end_read = 1;
      break;
    case 1:
      //kao za storage driver

      len = scnprintf(buf, BUFF_SIZE, "%d ", cache[pos]);
      if (copy_to_user(buffer, buf, len))
        return -EFAULT;
      
      pos++;
      if(pos == 2*colsize)
        end_read = 1;

      return len;
      
      break;

    default:
      printk(KERN_ERR "[READ] Invalid minor. \n");
      end_read = 1;
    }

  return len;
}

ssize_t seam_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
  char buf[BUFF_SIZE];
  int minor = MINOR(pfile->f_inode->i_rdev);
  if (copy_from_user(buf, buffer, length))
    return -EFAULT;
  buf[length - 1]='\0';

  int val = 0;
  int reset = 0;
  int ret;

  switch (minor)
    {
    case 0: //device seam
      sscanf(buf, "%d, %d, %d", &rowsize, &colsize, &start);
      printk(KERN_INFO "[WRITE BEGINS] ");
      printk(KERN_INFO "Rowsize: %d\nColsize: %d\nStart: %d\n", rowsize, colsize, start);

      if(start)
      {
        printk(KERN_INFO "[WRITE] Succesfully started seam_carving device\n");    
        ready = 0;
        hard_cem();
        ready = 1;
      }
      else
      {
        printk(KERN_INFO "[WRITE] Device initialized with rowsize and colsize only.\n");
      }


      printk(KERN_INFO "[WRITE ENDS] Succesfully wrote into seam device.\n");
      break;

    case 1:

      ret = sscanf(buf, "%d, %d", &val, &reset);
      if(ret == 2)
      {
        cache_waddr = cache_waddr % (2*colsize);
        cache[cache_waddr++] = val;
        printk(KERN_INFO "[WRITE] Succesfully wrote into cache. cache_waddr = %d, val = %d\n", cache_waddr, cache[cache_waddr-1]);

        if(reset)
        {
          cache_waddr = 0;
          hard_toggle_row = 0;
          printk(KERN_INFO "RESET OCCURED! cache_waddr = %d\n", cache_waddr);
          reset = 0;
        }
      }
      else
      {
        printk(KERN_WARNING "Wrong command format\nexpected: n\tn-value\n");
      }
      break;
    default:
      printk(KERN_INFO "[WRITE] Invalid minor. \n");
  }

  return length;
}

// ------------------------------------------
// INIT & EXIT
// ------------------------------------------

static int __init seam_init(void)
{
  printk(KERN_INFO "\n");
  printk(KERN_INFO "[INIT BEGIN] SEAM driver starting insmod.\n");

  int i;
  
  // cache initialization
  for(i = 0; i < CACHE_SIZE; i++)
    cache[i] = 0;

  hard_toggle_row = 0;
  cache_raddr = 0;
  cache_waddr = 0;
  start = 0;
  ready = 1;

////////////////////////////////////////////////////////////////////////////////
  if (alloc_chrdev_region(&my_dev_id, 0, 2, "seam_region") < 0){  
    printk(KERN_ERR "failed to register char device\n");
    return -1;
  }
  printk(KERN_INFO "char device region allocated\n");

////////////////////////////////////////////////////////////////////////////////
  my_class = class_create(THIS_MODULE, "seam_class");
  if (my_class == NULL){
    printk(KERN_ERR "failed to create class\n");
    goto fail_0;
  }
  printk(KERN_INFO "class created\n");
  

////////////////////////////////////////////////////////////////////////////////
  // Create IP
  if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),0), NULL, "xlnx,seam") == NULL) {
    printk(KERN_ERR "failed to create device seam\n");
    goto fail_1;
  }
  printk(KERN_INFO "device created - seam\n");

////////////////////////////////////////////////////////////////////////////////
  // Create DMA
  if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),1), NULL, "xlnx,dma_seam") == NULL) {
    printk(KERN_ERR "failed to create device DMA seam\n");
    goto fail_2;
  }
  printk(KERN_INFO "device created - DMA seam\n");

////////////////////////////////////////////////////////////////////////////////
	my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;

	if (cdev_add(my_cdev, my_dev_id, 2) == -1) 
	{
      printk(KERN_ERR "failed to add cdev\n");
      goto fail_2;
	}
  printk(KERN_INFO "cdev added\n");
  printk(KERN_INFO "[INIT 1 ENDS] Seam driver initialized.\n");

  return 0;

  fail_2:
    device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));  
  fail_1:
    class_destroy(my_class);
  fail_0:
    unregister_chrdev_region(my_dev_id, 1);
  return -1;

}

static void __exit seam_exit(void)
{
  printk(KERN_INFO "[EXIT BEGINS] Seam driver starting rmmod.\n");

  // Exit Device Module
	cdev_del(my_cdev);

  device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));

  class_destroy(my_class);
  unregister_chrdev_region(my_dev_id,1);

	printk(KERN_INFO "[EXIT ENDS] Seam driver exited.\n");
}

module_init(seam_init);
module_exit(seam_exit);