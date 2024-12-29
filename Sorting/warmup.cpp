// Author(kindof I guess): Theo Snoey
// Warmup debugging file to practice working with references/lost data etc...

#include "listnode.h"
#include "random.h"
#include "SimpleTest.h"
using namespace std;


// PROVIDED_TEST("Allocation counts: 1 new, 0 delete") {
//     ListNode* single = new ListNode(5, nullptr);
//     EXPECT_EQUAL(single->data, 5);
// }

// PROVIDED_TEST("Allocation counts: 10 new, 1 delete") {
//     ListNode* list = nullptr;
//     for (int i = 0; i < 10; i++) {
//         list = new ListNode(i, list);
//     }
//     delete list;
// }

// PROVIDED_TEST("Allocation counts: 1 new, 2 delete") {
//     ListNode* list = new ListNode(5, nullptr);
//     delete list;
//     delete list;  // delete same memory twice
// }


/*
 * This function is a buggy implementation of freeing a
 * linked list. In particular, on every iteration of the while
 * loop, the function attempts to access memory that was just
 * deallocated.
 */
void badDeallocate(ListNode* list) {
    ListNode *cur = list;
    while (cur != nullptr) {
        delete cur;
        cur = cur->next;  // BAD: accesses deallocated memory
    }
}

// PROVIDED_TEST("badDeallocate: low exposure to bug, short list") {
//     ListNode* list = nullptr;
//     for (int i = 0; i < 3; i++) {
//         list = new ListNode(i, list);
//     }
//     badDeallocate(list);
// }

/*
 * Buggy test that fails to properly initialize variables.
 * What are the consequences of using a garbage integer?
 * What are the consequences of using a garbage pointer?
 */
PROVIDED_TEST("Segmentation fault: use of uninitialized values") {
    ListNode* p = new ListNode; // allocate new node, don't assign data or next field, leave uninitialized
    p->data += 10;   // add 10 to garbage, what happens?
    p->next->data = 5; // dereference garbage address, what happens?
    delete p;
}
