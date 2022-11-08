//Bptree의 node내부에 있는 pair

public class Pair {
	
	int key;
	int value;
	Node ptr;
	
	public Pair() {
		key = 0;
		value = 0;
		ptr = null;
	}

	
	public Pair(int key, Node ptr) {
		this.key = key;
		this.value = -1;
		this.ptr = ptr;

	}
	
	public Pair(int key, int value) {
		super();
		this.key = key;
		this.value = value;
		this.ptr = null;
	}
	
	

}
