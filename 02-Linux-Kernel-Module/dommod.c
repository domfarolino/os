#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

/**
 * This struct represents a single
 * element in the linked list. Notice
 * we don't have a *next and *prev, instead
 * we have this list_head type named `list`.
 * The type list_head is defined in linux/types.h
 * that defines these pointers necessary to connect
 * it to other nodes in a list.
 */
struct Node {
  int data;
  struct list_head list;
};

// Create the head of our linked list
static LIST_HEAD(nodeList);

int dommod_init(void) {
  printk(KERN_INFO "Loading Dom's kernel module\n");

  /**
   * Create a few elements for our list
   * and insert them. The first part looks
   * very familiar. However we then have to
   * initialize the `list` of type list_head
   * that contains our *next and *prev pointers.
   * We do this with the macro INIT_LIST_HEAD. We
   * can then add this node to our list by using the
   * list_add_tail macro.
   */
  struct Node *firstNode, *secondNode;

  firstNode = kmalloc(sizeof(*firstNode), GFP_KERNEL);
  firstNode->data = 101;
  INIT_LIST_HEAD(&firstNode->list);

  secondNode = kmalloc(sizeof(*secondNode), GFP_KERNEL);
  secondNode->data = 102;
  INIT_LIST_HEAD(&secondNode->list);

  /**
   * This macro here allows takes the *next and *prev
   * members of our list node and uses them to connect
   * our node to the actual `nodeList` we've previously
   * set up.
   */
  list_add_tail(&firstNode->list, &nodeList);
  list_add_tail(&secondNode->list, &nodeList);

  /**
   * Now that we've added some nodes to our list
   * we can traverse the list with the list_for_each_entry
   * macro which accepts 3 parameters. First a pointer to the
   * struct being iterated over, then the head of the specific
   * list we're iterating over, then the name of the struct variable
   * containing the list_head structure.
   */
  struct Node *ptr;
  list_for_each_entry(ptr, &nodeList, list) {
    printk(KERN_INFO "ptr->data: %d\n", ptr->data);
    printk(KERN_INFO "Address of the node: %p\n", ptr);
  }

  return 0;
}

void dommod_exit(void) {
  printk(KERN_INFO "Removing Dom's kernel module\n");
  printk(KERN_INFO "We're going to iterate over our linked list, deleting elements as we come across them, giving memory back to the kernel\n");

  struct Node *ptr, *next;
  list_for_each_entry_safe(ptr, next, &nodeList, list) {
    list_del(&(ptr->list));
    kfree(ptr);
  }
}

module_init(dommod_init);
module_exit(dommod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Dom Module");
MODULE_AUTHOR("Dominic Farolino");
