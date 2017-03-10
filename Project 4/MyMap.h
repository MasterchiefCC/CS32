#ifndef MYMAP_H
#define  MYMAP_H
// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap():m_size(0),root(nullptr){}
	~MyMap() {
		helper_delete(root);
	}
	void clear() {
		helper_delete(root);
	}
	int size() const {
		return m_size;
	}
	void associate(const KeyType& key, const ValueType& value) {
		Node*temp = new Node(key, value);
		if (root == nullptr) { root = temp; m_size++; return; }

		else {
			Node*p = root;
			while (1) {
				if (temp->m_name > p->m_name)
					if (p->m_right == nullptr) {p->m_right = temp; m_size++; return;}
					else p = p->m_right;
				else if (temp->m_name < p->m_name)
					if (p->m_left == nullptr) { p->m_left = temp; m_size++; return; }
					else p = p->m_left;
				else if (temp->m_name == p->m_name) { p->m_value = value; delete temp; return; }
			}
		}
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const {
		Node*p = root;
		while (1) {
			if (p == nullptr)return nullptr;
			else if (key > p->m_name)p = p->m_right;
			else if (key < p->m_name)p = p->m_left;
			else if (key == p->m_name)return &p->m_value;
		}
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key){
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node{
	public:
		Node(KeyType name, ValueType value) {
			m_name = name;
			m_value = value;
			m_left = nullptr;
			m_right = nullptr;
		}
		KeyType m_name;
		ValueType m_value;
		Node* m_left;
		Node* m_right;
	};

	Node* root;
	int m_size;
	void helper_delete(Node*p) {
		if (p == nullptr)return;
		helper_delete(p->m_left);
		helper_delete(p->m_right);
		delete p;
	}
};
#endif // 

