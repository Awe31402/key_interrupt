#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define WORK_QUEUE_NAME "key_work"
#define INT_NAME "key_int"

static void got_char(struct work_struct *work);
DECLARE_WORK(key_work, got_char);
static struct workqueue_struct *key_work_queue;
static int count = 0;

static void got_char(struct work_struct *work)
{
	static unsigned char scancode;
	unsigned char status;

	scancode = inb(0x60);
	status = inb(0x64);
	printk(KERN_INFO "[%s] LINE %d: Scan code %x %s\n",
			__func__,
			__LINE__,
			scancode & 0x7f, 
			(scancode & 0x80)? "Released" : "Pressed");
	printk(KERN_INFO "[%s] LINE %d : jiffies = %ld, count = %d\n",
			__func__, __LINE__,
			jiffies, count);
}

static irqreturn_t irq_handler(int irq, void *dev_id)
{
	count++;
	printk(KERN_INFO "[%s] LINE %d : jiffies = %ld, count = %d\n",
		       	__func__,
			__LINE__,
			jiffies, count);
	queue_work(key_work_queue, &key_work);

	return IRQ_HANDLED;
}

static int keymodule_init(void)
{
	key_work_queue = create_singlethread_workqueue(WORK_QUEUE_NAME);
	free_irq(1, NULL);
	request_irq(1, irq_handler, IRQF_SHARED, INT_NAME, (void *) irq_handler);
	return 0;
}

static void keymodule_exit(void)
{
	free_irq(1, (void *) irq_handler);
	flush_workqueue(key_work_queue);
	destroy_workqueue(key_work_queue);
}

module_init(keymodule_init);
module_exit(keymodule_exit);
MODULE_LICENSE("Dual BSD/GPL");
