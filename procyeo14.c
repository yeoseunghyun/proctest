#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define BUFSIZE 512 
#define custommin(a,b) (((a)<(b)) ? (a) : (b))

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("yeo14");

static char* inbuf;
static unsigned long buffersize=0;

static struct proc_dir_entry *ent;

static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
	if(count > BUFSIZE)
		buffersize= BUFSIZE;
	else buffersize=count;

	if(copy_from_user(inbuf, ubuf, buffersize))
	{
		printk(KERN_ALERT "copy from user in mywrit  error\n");
		return -EFAULT;
	}
	return buffersize;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	static int done =0;
	if(done)
	{
		done=0;
		return 0;
	}
	done=1;

	if(copy_to_user(ubuf,inbuf,buffersize))
	{
		printk(KERN_ALERT "copy to user in myread error\n");
		return -EFAULT;
	}
	return buffersize;
}
int myopen(struct inode *inode,struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}
int myrelease(struct inode * inode,struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}

static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.read = myread,
	.write = mywrite,
	.open = myopen,
	.release = myrelease,
};

static int simple_init(void)
{
	ent=proc_create("procyeo14",0,NULL,&myops);
	inbuf=kmalloc(BUFSIZE,GFP_KERNEL);
	printk(KERN_ALERT "simple_test_by_yeo14\n");
	memset(inbuf,0,BUFSIZE);
	return 0;
}

static void simple_cleanup(void)
{
	proc_remove(ent);
	printk(KERN_WARNING "bye ...\n");
	kfree(inbuf);
}

module_init(simple_init);
module_exit(simple_cleanup);
