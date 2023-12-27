#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Denys Makernko");
MODULE_DESCRIPTION("Hello, this is LabWork №3");
MODULE_LICENSE("Dual BSD/GPL");

// Структура для зберігання даних та списку
static struct list_head hello_list_head;

struct hello_data {
    struct list_head list;
    ktime_t time;
};

// Параметр модуля - кількість разів, яку виведеться "Hello, world!"
static uint count = 1;
module_param(count, uint, S_IRUGO);
MODULE_PARM_DESC(count, "Count of times to print \"Hello, world!\"");

// Ініціалізація модуля
static int __init hello_init(void)
{
    int i;
    struct hello_data *ptr;

    // Перевірка параметрів модуля
    if (count == 0) {
        pr_warn("Count is 0.\n");
    }

    if (count >= 5 && count <= 10) {
        pr_warn("Count is between 5 and 10.\n");
    }

    if (count > 10) {
        pr_err("Count is greater than 10. \nExiting with error.\n");
        return -EINVAL;
    }

    // Ініціалізація списку
    INIT_LIST_HEAD(&hello_list_head);

    // Заповнення списку та виведення "Hello, world!"
    for (i = 0; i < count; ++i) {
        ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
        if (!ptr)
            return -ENOMEM;

        ptr->time = ktime_get();
        list_add_tail(&ptr->list, &hello_list_head);

        pr_emerg("Hello, world!\n");
    }

    return 0;
}

// Видалення модуля
static void __exit hello_exit(void)
{
    struct hello_data *ptr, *temp;

    // Виведення часу для кожного запису в списку та видалення його
    list_for_each_entry_safe(ptr, temp, &hello_list_head, list) {
        pr_info("Time: %lld ns\n", ktime_to_ns(ptr->time));
        list_del(&ptr->list);
        kfree(ptr);
    }
}

// Вказівки на функції ініціалізації та виходу
module_init(hello_init);
module_exit(hello_exit);

