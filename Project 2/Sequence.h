#ifndef SEQUENCE_H
#define SEQUENCE_H      

#include <stdio.h>
#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	/*Sequence();    
	Sequence& operator=(const Sequence& other);

	~Sequence();

	Sequence(const Sequence&other);//copy constructor

	bool empty() const;// Return true if the sequence is empty, otherwise false.

	int size() const;    // Return the number of items in the sequence.

	bool insert(int pos, const ItemType& value);
	// Insert value into the sequence so that it becomes the item at
	// position pos.  The original item at position pos and those that
	// follow it end up at positions one higher than they were at before.
	// Return true if 0 <= pos <= size() and the value could be
	// inserted.  (It might not be, if the sequence has a fixed capacity,
	// (e.g., because it's implemented using a fixed-size array) and is
	// full.)  Otherwise, leave the sequence unchanged and return false.
	// Notice that if pos is equal to size(), the value is inserted at the
	// end.

	int insert(const ItemType& value);
	// Let p be the smallest integer such that value <= the item at
	// position p in the sequence; if no such item exists (i.e.,
	// value > all items in the sequence), let p be size().  Insert
	// value into the sequence so that it becomes the item at position
	// p.  The original item at position p and those that follow it end
	// up at positions one higher than before.  Return p if the value
	// was actually inserted.  Return -1 if the value was not inserted
	// (perhaps because the sequence has a fixed capacity and is full).

	bool erase(int pos);
	// If 0 <= pos < size(), remove the item at position pos from
	// the sequence (so that all items that followed this item end up at
	// positions one lower than they were at before), and return true.
	// Otherwise, leave the sequence unchanged and return false.

	int remove(const ItemType& value);
	// Erase all items from the sequence that == value.  Return the
	// number of items removed (which will be 0 if no item == value).

	bool get(int pos, ItemType& value) const;
	// If 0 <= pos < size(), copy into value the item at position pos
	// in the sequence and return true.  Otherwise, leave value unchanged
	// and return false.

	bool set(int pos, const ItemType& value);
	// If 0 <= pos < size(), replace the item at position pos in the
	// sequence with value and return true.  Otherwise, leave the sequence
	// unchanged and return false.

	int find(const ItemType& value) const;
	// Let p be the smallest integer such that value == the item at
	// position p in the sequence; if no such item exists, let p be -1.
	// Return p.

	void swap(Sequence& other);
	// Exchange the contents of this sequence with the other one.
	*/
public:
	Sequence();// Create an empty sequence (i.e., one whose size() is 0).
	~Sequence();//destructor
	Sequence(const Sequence&other);
	Sequence& operator=(const Sequence&other);
	bool empty() const;
	int size() const;
	bool insert(int pos, const ItemType& value);
	int insert(const ItemType& value);
	bool erase(int pos);
	int remove(const ItemType& value);
	bool get(int pos, ItemType& value) const;
	bool set(int pos, const ItemType& value);
	int find(const ItemType& value) const;
	void swap(Sequence& other);

private:
	struct Node{
		ItemType m_value;
		Node* m_next;
		Node* m_previous;
	};
	int m_size;
	Node* head;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);


#endif 
