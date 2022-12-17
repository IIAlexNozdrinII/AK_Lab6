#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Alexander Nozdrin IO-04");
MODULE_DESCRIPTION("AK Lab 6");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int greetings_number = 1;

module_param(greetings_number, uint, S_IRUGO);

MODULE_PARM_DESC(greetings_number, "Parameter specifies the number of times 'Hello, world!' will be printed");

struct event_list {
    struct list_head list;
    ktime_t event_time;
};

static struct list_head event_list_head;

static LIST_HEAD(event_list_head);

void new_event(void);

static int __init hello_init(void)
{
    if (greetings_number == 0 || (greetings_number >= 5 && greetings_number <= 10)) {
        printk(KERN_WARNING "Parameter is in the range from 5 to 10 or 0");
    }

    if (greetings_number > 10) {
        printk(KERN_ERR "Parameter is too large (greater than 10)");
        return -EINVAL;
    }
  
    int i;
    for (i = 0; i < greetings_number; i++) {
        printk(KERN_EMERG "Hello, World!\n");
        new_event();
    }
  return 0;
}

static void __exit hello_exit(void) {
  struct event_list *md, *tmp;
  list_for_each_entry_safe(md, tmp, &event_list_head, list) {
    printk(KERN_EMERG "Time: %lld\n", md->event_time);
    list_del(&md->list);
    kfree(md);
  }
}

void new_event(void) {
    struct event_list *element = NULL;
    element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
    element->event_time = ktime_get();
    list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);
