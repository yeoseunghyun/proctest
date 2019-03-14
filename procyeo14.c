#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#define BUFSIZE  1024


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran B.H");

static char inbuf[64]="";
//module_param(inbuf,char,0660);

//static int irq=20;
//module_param(irq,int,0660);

//static int mode=1;
//module_param(mode,int,0660);

static struct proc_dir_entry *ent;

static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
	int num,c,i,m;
	char buf[BUFSIZE];
	if(*ppos > 0 || count > BUFSIZE)
		return -EFAULT;
	if(copy_from_user(buf, ubuf, count))
		return -EFAULT;
//	inbuf = kmalloc(GFP_KERNEL, count);
	num = sscanf(buf,"%s",inbuf);
//	if(num != 2)
//		return -EFAULT;
//	irq = i; 
//	mode = m;
	c = strlen(inbuf);
	*ppos = c;
	return c;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	char buf[BUFSIZE];
	int len=0;
	if(*ppos > 0 || count < BUFSIZE)
		return 0;
	len += sprintf(buf,"%s",inbuf);
	//len += sprintf(buf + len,"mode = %d\n",mode);

	if(copy_to_user(ubuf,buf,len))
		return -EFAULT;
	*ppos = len;
	return len;
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
	printk(KERN_ALERT "simple_test_by_yeo14\n");
	return 0;
}

static void simple_cleanup(void)
{
	proc_remove(ent);
//	kfree(inbuf);
	printk(KERN_WARNING "bye ...\n");
}

module_init(simple_init);
module_exit(simple_cleanup);
