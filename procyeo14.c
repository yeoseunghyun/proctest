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

static struct proc_dir_entry *ent;

static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
	/*	int num,c,i,m;
		unsigned long len=custommin(count,BUFSIZE-1);
		if(*ppos > 0 || count > BUFSIZE)
		return -EFAULT;
		if(copy_from_user(inbuf, ubuf, count))
		return -EFAULT;
		c = strlen(inbuf);
	 *ppos = c;*/

	if(count > BUFSIZE)
		count = BUFSIZE;
	if(copy_from_user(inbuf, ubuf, count))
	{
		printk(KERN_ALERT "copy from user in mywrit  error\n");
		return -EFAULT;
	}
	return count;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	int len=0;
	//f(*ppos == 0 || count < BUFSIZE)
	//return 0;
	//len += sprintf(buf,"%s",inbuf);
	//len += sprintf(buf + len,"mode = %d\n",mode);

	if(copy_to_user(ubuf,inbuf,BUFSIZE))
	{
		printk(KERN_ALERT "copy to user in myread error\n");
		return -EFAULT;
	}
	//*ppos = len;
	return count;
}

static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.read = myread,
	.write = mywrite,
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
	//	kfree(inbuf);
	printk(KERN_WARNING "bye ...\n");
	kfree(inbuf);
}

module_init(simple_init);
module_exit(simple_cleanup);
