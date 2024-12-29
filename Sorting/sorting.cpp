// Author: Theo Snoey
// Sorting.cpp uses linked lists to perform differnt kinds of sorting algorithms and tests their relative efficiency

#include "listnode.h"
#include "vector.h"
#include "SimpleTest.h"
#include "random.h"
using namespace std;




// This function, splitRun takes a front pointer to a Listnode and essentially cuts off the first sorted sublist
// in that larger list. It returns the pointer to the rest of the list and the initial pointer passed by refernce as a parameter
// points to the split-off list.
ListNode* splitRun(ListNode*& front){

    ListNode *cur = front;

    if (cur == nullptr){
        return nullptr;
    }

    // keep going until the next pointer points to something smaller...
    while (cur->next != nullptr && cur->data <= cur->next->data){
        cur = cur->next;
    }

    ListNode *newStart = cur->next;
    cur->next = nullptr;
    return newStart;
}


// this function, merge, takes two pointers to the head of linked lists, and merges them both into one single sorted list
// returning the pointer to the head of that new merged list.
ListNode* merge(ListNode *& cur1, ListNode *& cur2){

    // save computation by directly returning non-nullptr list header
    if (cur1 == nullptr){
        return cur2;
    }
    if (cur2 == nullptr){
        return cur1;
    }

    ListNode *thisSpot = nullptr;
    ListNode *startNode = nullptr;

    // find out which of the two header nodes has a greater data value
    // and assign it as the start of our merged list
    if (cur1->data <= cur2->data){
        thisSpot = cur1;
        cur1 = cur1->next;
    }
    else {
        thisSpot = cur2;
        cur2 = cur2->next;
    }

    // save startNode pointer, will be returned at the end
    startNode = thisSpot;

    // while both cur1 and cur2 don't point to null,
    // this should exhaust one of the lists completely if both lists
    // aren't the same length
    while (cur1 != nullptr && cur2 != nullptr){
        // compare both, add smaller one and update accordingly
        if (cur1->data <= cur2->data){
            thisSpot->next = cur1;
            cur1 = cur1->next;
        }
        else {
            thisSpot->next = cur2;
            cur2 = cur2->next;
        }
        thisSpot = thisSpot->next;
    }


    //add remaining objects if there are any, it should be only one of these

    if (cur2 != nullptr){

        while (cur2 != nullptr){
            thisSpot->next = cur2;
            cur2 = cur2->next;
            thisSpot = thisSpot->next;
        }
    }

    else if (cur1 != nullptr){

        while (cur1 != nullptr){
            thisSpot->next = cur1;
            cur1 = cur1->next;
            thisSpot = thisSpot->next;
        }
    }

    return startNode;
}

/*
 * This function, runSort, performs the runSort algorithm. It takes a listNode pointer to the head of a linked list "front"
 * and calls splitRun to split the first sorted list, then continually calls split run and merge to merge new sorted lists
 * to a larger result list, The pointer, front, passed by reference is assigned to the head of that new sorted list.
 */
void runSort(ListNode*& front) {

    // set up the result so that while loop code is simpler...
    ListNode *result = nullptr;

    ListNode *listOne = splitRun(front);

    result = merge(result, front);

    while (listOne != nullptr){
        ListNode *listTwo = splitRun(listOne);
        result = merge(result, listOne);

        listOne = listTwo;
    }
    // have to assign to initial reference
    front = result;
}



// This function, partition, takes a listnode front, smallerStart, equal Start, and LargerStart all passed by reference.
// It then takes the front header assigmment and uses that first Listnode as a pivotal value, a comparison to use to split the
// given linked list into 3 sublists smaller than, equal to, and larger than that value. These lists' headers are assigned to
// the other params passed by reference.
void partition(ListNode*& front, ListNode*& smallerStart, ListNode*& equalStart, ListNode*& largerStart){
    int pivotalVal = front->data;
    ListNode *smaller = nullptr;
    ListNode *equal = nullptr;
    ListNode *larger = nullptr;

    ListNode *cur = front;

    while (cur != nullptr){
        if (cur->data < pivotalVal){
            // if nothing has been assigned to the list, assign it to the current header pointer
            if (smaller == nullptr){
                smaller = cur;
                smallerStart = cur;
            }
            // if the list already has a header, continue to assign to next and updated pointers
            else {
                smaller->next = cur;
                smaller = smaller->next;
            }
        }
        if (cur->data == pivotalVal){
            if (equal == nullptr){
                equal = cur;
                equalStart = cur;
            }
            else {
                equal->next = cur;
                equal = equal->next;
            }
        }
        if (cur->data > pivotalVal){
            if (larger == nullptr){
                larger = cur;
                largerStart = cur;
            }
            else {
                larger->next = cur;
                larger = larger->next;
            }
        }
        // no matter what update cur to cur->next to keep going through the cur/front linked list
        cur = cur->next;
    }

    //set the ends of all lists to nullptr, have to check if og is = nullptr to not have dereferencing
    if (smaller != nullptr){
        smaller->next = nullptr;
    }
    if (equal != nullptr){
        equal->next = nullptr;
    }
    if (larger != nullptr){
        larger->next = nullptr;
    }

}


// this function, concatenate, takes two pointers to a listNodes, that are headers for their respective linked lists,
// and then concatenates both lists, assigning the new single list header to the first parameter passed by reference.
void concatenate(ListNode*& First, ListNode*& Second){
    if (First == nullptr){
        First = Second;
        return;
    }

    ListNode *cur = First;

    while (cur->next != nullptr){
        cur = cur->next;
    }

    cur->next = Second;
}


/*
 * This function, quickSort, takes a pointer to the header of a linked list and performs the quickSort algorithm on that
 * list assigning the new sorted list to the same parameter "front" passed by reference. Specifically, this algorithm
 * recursively separates a given list into 3 sublists calling partition, then recalls quicksort on those sublists, all these
 * sublists are then concatenated in sorted order.
 */
void quickSort(ListNode*& front){
    // base cases: first is to avoid derefernceing
    // second: so that we stop when front only has one node left
    if (front == nullptr){
        return;
    }
    if (front->next == nullptr){
        return;
    }

    ListNode *smallerStart = nullptr;
    ListNode *equalStart = nullptr;
    ListNode *largerStart = nullptr;

    // partition then recall...
    partition(front, smallerStart, equalStart, largerStart);

    quickSort(smallerStart);
    quickSort(largerStart);

    // after base cases met, we add the resulting sorted parts...
    concatenate(smallerStart, equalStart);
    concatenate(smallerStart, largerStart);
    front = smallerStart;
}



/* * * * * * Test Code Below This Point * * * * * */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
    cout << "{";
    for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
        cout << cur->data;
        if (cur->next != nullptr) {
           cout << ", ";
        }
    }
    cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    ListNode *cur = front;

    while (cur != nullptr){
        ListNode *next = cur->next;
        delete cur;
        cur = next;
    }

}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values) {
    if (values.isEmpty()){
        return nullptr;
    }

    ListNode *head = new ListNode;
    ListNode *cur = head;
    head->data = values[0];

    for (int i = 1; i < values.size(); i++){
        cur->next = new ListNode;
        cur->next->data = values[i];
        cur = cur->next;
    }

    return head;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v) {

    int i = 0;
    ListNode *cur = front;
    while (cur != nullptr && i < v.size()){
        if (v[i] != cur->data){
            return false;
        }

        i++;

        cur = cur->next;
    }

    if (cur == nullptr && i == v.size()){
        return true;
    }

    return false;
}


//_________________________________STUDENT TESTS_________________________________________________

STUDENT_TEST("Testing my merge helper function") {
    /* Initialize vector to sequence of values */
    Vector<int> values1 = {1, 3, 5, 7};

    Vector<int> values2 = {2, 4, 6, 8};

    Vector<int> mergedValues = {1, 2, 3, 4, 5, 6, 7, 8};
    /* Create equivalent list to use as test input */
    ListNode* list1 = createList(values1);
    ListNode* list2 = createList(values2);


    ListNode* mergedList = merge(list1, list2);

    printList(mergedList);

    EXPECT(areEquivalent(mergedList, mergedValues));

    deallocateList(mergedList);
}


STUDENT_TEST("Testing my splitrun Helper function") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 3, 5, 7, 56, 48, 90, -23, 48};
    Vector<int> valuesSplit = {1, 3, 5, 7, 56};

    ListNode *list = createList(values);

    ListNode *secondList= splitRun(list);

    EXPECT(areEquivalent(list, valuesSplit));

    deallocateList(list);
    deallocateList(secondList);
}


STUDENT_TEST("Testing my runSort function") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 3, 5, 7, 56, 48, 90, -23, 48};


    ListNode *list = createList(values);

    values.sort();

    runSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}


STUDENT_TEST("Testing my RunSort function on empty list") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {};

    ListNode *list = createList(values);

    values.sort();

    runSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}


STUDENT_TEST("Testing my runSort function on single element list") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {5};

    ListNode *list = createList(values);

    values.sort();

    runSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}

STUDENT_TEST("Testing my runSort function when already sorted") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {50, -6, 81, 10, -14, 18};

    values.sort();

    ListNode *list = createList(values);

    runSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}

STUDENT_TEST("Testing my runSort function with random vals") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 1000; i++){
        values += randomInteger(-100, 100);
    }

    values.sort();

    ListNode *list = createList(values);

    runSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}



STUDENT_TEST("Testing my concatenate function") {
    Vector<int> values1 = {1, 3, 5, 7};
    Vector<int> values2 = {9, 10, 12, 15};
    Vector<int> values3 = {1, 3, 5, 7, 9, 10, 12, 15};


    values1.sort();
    values2.sort();

    ListNode *list1 = createList(values1);
    ListNode *list2 = createList(values2);

    concatenate(list1, list2);

    EXPECT(areEquivalent(list1, values3));

    deallocateList(list1);
}



STUDENT_TEST("Testing my partition function") {
    Vector<int> values = {6, 2, 3, 5, 6, 6, 90, 91, 48};
    ListNode *front = createList(values);
    ListNode *smallerStart = nullptr;
    ListNode *equalStart = nullptr;
    ListNode *largerStart = nullptr;

    Vector<int> values1 = {2, 3, 5};
    Vector<int> values2 = {6, 6, 6};
    Vector<int> values3 = {90, 91, 48};

    partition(front, smallerStart, equalStart, largerStart);

    EXPECT(areEquivalent(smallerStart, values1));
    EXPECT(areEquivalent(equalStart, values2));
    EXPECT(areEquivalent(largerStart, values3));

    deallocateList(smallerStart);
    deallocateList(equalStart);
    deallocateList(largerStart);
}



STUDENT_TEST("Testing my QuickSort function") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 3, 5, 7, 56, 48, 90, -23, 48};

    ListNode *list = createList(values);

    values.sort();

    quickSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}


STUDENT_TEST("Testing my QuickSort function on empty list") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {};

    ListNode *list = createList(values);

    values.sort();

    quickSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}


STUDENT_TEST("Testing my QuickSort function on single element list") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {5};

    ListNode *list = createList(values);

    values.sort();

    quickSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}

STUDENT_TEST("Testing my QuickSort function when already sorted") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {50, -6, 81, 10, -14, 18};

    values.sort();

    ListNode *list = createList(values);

    quickSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}

STUDENT_TEST("Testing my QuickSort function with random vals") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 1000; i++){
        values += randomInteger(-100, 100);
    }

    values.sort();

    ListNode *list = createList(values);

    quickSort(list);

    EXPECT(areEquivalent(list, values));

    deallocateList(list);
}



STUDENT_TEST("Time Trial for RunSort") {
    /* Initialize vector to sequence of values */

    for (int size = 10000; size <= 80000; size *= 2){
        // generate random vector of size, sizze
        Vector<int> values;
        for (int i = 0; i < size; i++){
            values += randomInteger(-100, 100);
        }

        ListNode *list = createList(values);

        TIME_OPERATION(size, runSort(list));
        deallocateList(list);
    }
}

STUDENT_TEST("Time Trial for RunSort presorted with imperfections") {
    /* Initialize vector to sequence of values */

    for (int size = 10000; size <= 80000; size *= 2){

        // generate random vector of size, sizze
        Vector<int> values;
        for (int i = 0; i < size; i++){
            values += randomInteger(-100, 100);
        }

        values.sort();

        // add imperfections to the vector's sorted structure
        for (int i = 0; i < size; i += size/100){
            values[i] = 6;
        }

        ListNode *list = createList(values);

        TIME_OPERATION(size, runSort(list));
        deallocateList(list);
    }
}


STUDENT_TEST("Time Trial for QuickSort") {
    /* Initialize vector to sequence of values */

    for (int size = 10000; size <= 80000; size *= 2){
        // generate random vector of size, sizze
        Vector<int> values;
        for (int i = 0; i < size; i++){
            values += randomInteger(-100, 100);
        }

        ListNode *list = createList(values);

        TIME_OPERATION(size, quickSort(list));
        deallocateList(list);
    }
}


STUDENT_TEST("Time Trial for QuickSort") {
    /* Initialize vector to sequence of values */

    for (int size = 10000; size <= 80000; size *= 2){
        // generate random vector of size, sizze
        Vector<int> values;
        for (int i = 0; i < size; i++){
            values += randomInteger(-100, 100);
        }

        ListNode *list = createList(values);

        TIME_OPERATION(size, quickSort(list));
        deallocateList(list);
    }
}

STUDENT_TEST("Time Trial for QuickSort with worst possible runtime") {
    /* Initialize vector to sequence of values */

    for (int size = 10000; size <= 80000; size *= 2){
        // generate random vector of size, sizze
        Vector<int> values;
        for (int i = 0; i < size; i++){
            values += randomInteger(-100, 100);
        }

        values.sort();

        //make reverse sorted order

        Vector<int> reverseValues;

        for (int j = size - 1; j >= 0; j--){
            reverseValues += values[j];
        }

        ListNode *list = createList(reverseValues);

        TIME_OPERATION(size, quickSort(list));
        deallocateList(list);
    }
}

STUDENT_TEST("Compare QuickSort on list to Vector.sort() func") {
    /* Initialize vector to sequence of values */

    for (int size = 10000; size <= 80000; size *= 2){
        // generate random vector of size, size
        Vector<int> values;
        for (int i = 0; i < size; i++){
            values += randomInteger(-100, 100);
        }

        ListNode *list = createList(values);

        TIME_OPERATION(size, quickSort(list));
        TIME_OPERATION(size, values.sort());

        deallocateList(list);
    }
}




//_______________________________PROVIDED_TESTS_________________________________________________

PROVIDED_TEST("Confirm correctness of test utility functions.") {
    /* Manually construct testList 1 -> 2 -> 3 */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++) {
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

