import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.LinkedList;
import java.util.Scanner;

//Bptree�� Node

public class Node {
	
	static int degree = 0;
	int num = 0;
	LinkedList<Pair> pair = new LinkedList<Pair>();
	Node ptr = null;
	boolean leaf;
	boolean root = false;
	int height = 0; //leaf�� 0, leaf�κ�����  ���̸� ��Ÿ��.
	
	
	public void CreateBptree(String file, int num)
	{
		try {
			PrintWriter outStream = new PrintWriter(new FileOutputStream(file));
			Node.degree = num;
			outStream.println(num);
			outStream.close();
//			System.out.println("finish create file");
//			System.out.println("degree is " + degree);
			
		} catch (FileNotFoundException e) {
//			System.out.println("Create error");
		}
	}
	
	public void InsertData(String file, String data, int degree)
	{
		try
		{
			Scanner number = new Scanner(new FileInputStream(file));
			int num = number.nextInt();
			number.close();
			Node.degree = num;
//			System.out.println("degree is "+num);
			
			Scanner inStream = new Scanner(new FileInputStream(data)); //������Ʈ ���� �ȿ� file�� �����ϰ� ����.
//			outStream.println("Insert");
			while(inStream.hasNext())
			{
				String str = inStream.next();
				String[] pair = str.split(",");
				InsertKey(num, file, Integer.parseInt(pair[0]), Integer.parseInt(pair[1]));
//				outStream.println(pair[0] +","+pair[1]);
			}
			
			PrintWriter outStream = new PrintWriter(new FileOutputStream(file));
			Node temp = this;
			while(!temp.leaf)
			{
				temp = temp.pair.getFirst().ptr;
			}
			outStream.println(num);
			while(temp != null)
			{
				for(int i = 0; i < temp.num; i++)
					outStream.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
					//System.out.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
				temp = temp.ptr;
			}
			
			outStream.close();
			inStream.close();
//			System.out.println("finish insert");
			
		} catch (FileNotFoundException e) 
		{
//			System.out.println("Insert error");
		}
	}
	
	public void DeleteData(String file, String data, int degree)
	{
		try
		{
			Scanner number = new Scanner(new FileInputStream(file));
			int num = number.nextInt();
			Node.degree = num;
			
			while(number.hasNext())
			{
				String str = number.next();
				String[] pair = str.split(",");
				InsertKey(Node.degree, file, Integer.parseInt(pair[0]), Integer.parseInt(pair[1]));
//					outStream.println(pair[0] +","+pair[1]);
			}
			number.close();
//			outStream.println("Delete");
			
			Scanner inStream = new Scanner(new FileInputStream(data));
			while(inStream.hasNext())
			{
				String str = inStream.next();
				int key = Integer.parseInt(str);
				DeleteKey(Node.degree, file, key);
				//outStream.println(key);
//				System.out.println("Delete " + key);
			}
			
			inStream.close();
			
			PrintWriter outStream = new PrintWriter(new FileOutputStream(file));
			//outStream.println(num);
			outStream.println(num);
			Node temp = this;
			while(!temp.leaf)
			{
				temp = temp.pair.getFirst().ptr;
			}

			while(temp != null)
			{
				for(int i = 0; i < temp.num; i++)
					outStream.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
					//System.out.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
				temp = temp.ptr;
			}
			
			outStream.close();
//			System.out.println("finish delete");
		} catch (FileNotFoundException e) 
		{
//			System.out.println("delete error");
		}
	}
	
	public void DeleteKey(int degree, String file, int key) throws FileNotFoundException
	{
		//this == root
//		PrintWriter outStream = new PrintWriter(new FileOutputStream(file,true));
		
		boolean exist = false;
		if(this.num != 0)
		{
			Node look = this;
			while(!look.leaf)
			{
				look = look.pair.getFirst().ptr;
			}
		
			while(look != null)
			{
				for(int i = 0; i < look.num; i++)
					if(look.pair.get(i).key == key)
						exist = true;
				look = look.ptr;
			}
			
			if(!exist)
				return;
		}
		
 		int minKeyNum = (degree-1)/2; //node�ȿ� �ִ� key�� ������ �� ������ �������� underflow�� �߻���.
		
		Node target = FindLeafNode(key); //key�� ����ִ� leaf node�� ã��
		Node parent = target.FindParentNode(this);
		//�ϴ� key�� ��������
		int num = 0;
		for(int i = 0; i < target.num; i++)
		{
			if(target.pair.get(i).key == key)
			{
				num = i;
			}
		}
		
		/******************�߰��� �κ�********************/
		/******�� �κ��� index node�� ������*****************/
		Node parentnode = target.FindNonLeafNode(this,key); //���� �������� �𸣴� parent
		Node firstparent = target.FindParentNode(this); //���̰� 1�� parent
		
		//���̰� 1�� parent�� ���� ������ leaf node�� first key�� index node�� key�� �ȴ�
		//tree���� ���� �����ʿ� �ִ� leaf node�� first key�� index node�� �������� �ʱ� ������ parentnode�� null�� �� �� �ִ�
		if(parentnode != null && num == 0 && target == firstparent.pair.getFirst().ptr)
		{
			for(int i = 0; i < parentnode.num; i++)
			{
				if(parentnode.pair.get(i).key == key)
				{
					if(target.num == 1)
						parentnode.pair.get(i).key = target.ptr.pair.getFirst().key;
					else
						parentnode.pair.get(i).key = target.pair.get(1).key;
				}
			}
		}
		
		//borrow�� merge�� �Ͼ�� �ʴµ� index node���� key�� �ٲ����ϴ� ���
		if(num == 0 && target.num > minKeyNum)
		{
			for(int i = 0; i < firstparent.num; i++)
			{
				if(firstparent.pair.get(i).key == key)
					firstparent.pair.get(i).key = target.pair.get(1).key;
			}
		}
		
		
		
//		if(num == 0 && parentnode != null && parentnode.height > 1)
//		{
//			if(target.num != 1)
//			{
//				for(int i = 0; i < parentnode.num; i++)
//				{
//						if(parentnode.pair.get(i).key == key)
//							parentnode.pair.get(i).key = target.pair.get(1).key;
//				}
//			}
//			else
//			{
//				if(target != parent.ptr)
//				{
//					for(int i = 0; i < parentnode.num; i++)
//					{
//						if(parentnode.pair.get(i).key == key)
//							parentnode.pair.get(i).key = target.ptr.pair.getFirst().key;
//					}
//				}	
//			}
//		}
		
//		Node parentnode = target.FindNonLeafNode(this, key);	
//		if(num == 0 && parentnode != null /*&& target.num != 1*/) //non leaf node�� index �ٲ��ֱ�
//		{
//			//Node parentnode = target.FindNonLeafNode(this, key);
//			//������ key�� index node�� ������ key�� �ٲ���
//			if(target.num != 1)
//			{
//				for(int i = 0; i < parentnode.num; i++)
//				{
//						if(parentnode.pair.get(i).key == key)
//							parentnode.pair.get(i).key = target.pair.get(1).key;
//				}
//			}
//			else
//			{
//				if(parent.num != 1)
//				{
//					for(int i = 0; i < parentnode.num; i++)
//					{
//						if(parentnode.pair.get(i).key == key)
//						{
//							if(parentnode.pair.get(i+1).key != key) //����� ������ ������������..�ٵ� �״�� �θ� merge�Ҷ� ��Һ񱳰� �ȵ�
//							{
//								parentnode.pair.get(i).key = target.ptr.pair.getFirst().key;
//							}
//						}
//					}
//				}
//				
//				if(target.num == 1 && target != parent.ptr /*&& parentnode.height > 1*/)
//				{
//					for(int i = 0; i < parentnode.num; i++)
//					{
//						if(parentnode.pair.get(i).key == key)
//							parentnode.pair.get(i).key = target.ptr.pair.getFirst().key;
//					}
//				}	
//			}
//		}
		
		target.pair.remove(num); //key���� ���� pair�� �߰��ϸ� ����.
		target.num -= 1;
		
		
		//Node parent = target.FindParentNode(key);
		//Node parent = target.FindParentNode(this);
		if(target.num < minKeyNum) //underflow �߻�
		{
			//if(target.ptr != null && target.FindParentNode(this) == target.)
			if(target.ptr != null && parent == target.ptr.FindParentNode(this) && target.ptr.num > minKeyNum)
			//if(FindParentNode(key) == FindParentNode(target.ptr.pair.getFirst().key) && target.ptr.num >= minKeyNum) //right sibling���� key borrow
			{
//				System.out.println("borrow key from sibling");
				Node sibling = target.ptr;
				Pair borrow = sibling.pair.getFirst();
				sibling.pair.removeFirst();
				sibling.num -= 1;
				target.pair.addLast(borrow);
				target.num += 1;
					
				//rootnode ����
				
				/******************�߰��� �κ�********************/
				if(parent.num != 0)
				{
					for(int i = 0; i < parent.num-1; i++)
					{
						parent.pair.get(i).key = parent.pair.get(i+1).ptr.pair.getFirst().key;
					}
					parent.pair.getLast().key = parent.ptr.pair.getFirst().key;
				}
				
				
				
//				Node parentnode = target.FindNonLeafNode(this, target.pair.getLast().key);
//				int count = 0;
//				for(int i = 0; i < parent.num; i++)
//				{
//					if(parent.pair.get(i).key == target.pair.getLast().key)
//					{
//						count = i; //���� ���ڰ� parent�ȿ� �����Ѵٸ� �����ؼ� �� ������ ���ڸ� �����ϰ� ��.
//					}
//				}
//				parent.pair.get(count).key = sibling.pair.getFirst().key;
//				
//				/******************�߰��� �κ�********************/
//				if(count != 0)
//				{
//					parent.pair.get(count-1).key = parent.pair.get(count).ptr.pair.getFirst().key;
//				}
				
			}
			//2. merge
			else
			{
//				System.out.println("merge in leaf node");
				//right child�� merge�ϰ� split key ����
				//split key
				//Node parent = FindParentNode(key);
				parent = target.MergeInLeafNode(this, parent, key, degree);
				
				if(!parent.leaf && parent.root)
				{
					this.height = parent.height;
					this.leaf = parent.leaf;
					this.num = parent.num;
					this.pair = parent.pair;
					this.ptr = parent.ptr;
					this.root = parent.root;
				}
			}
		
			
			//leafnode �������� underflow�� ���� �۾��� �����ߴµ��� underflow�� �߻��Ѵٸ�
			if(parent.num < minKeyNum)
			{
				parent = UnderflowInNonLeafNode(parent, target, minKeyNum);
				
				if(!parent.leaf && parent.root)
				{
					this.height = parent.height;
					this.leaf = parent.leaf;
					this.num = parent.num;
					this.pair = parent.pair;
					this.ptr = parent.ptr;
					this.root = parent.root;
				}
				
			}
		}
		
//		if(!parent.leaf && parent.root)
//		{
//			this.height = parent.height;
//			this.leaf = parent.leaf;
//			this.num = parent.num;
//			this.pair = parent.pair;
//			this.ptr = parent.ptr;
//			this.root = parent.root;
//		}
		
//		outStream.close();
		
	}
	
	
	public Node UnderflowInNonLeafNode(Node parent, Node target, int minKeyNum)
	{
		//this == root
//		System.out.println("borrow key from parent");
		//Node parent = FindParentNode(key);
		//1. parent���׼� ������
		//parent�� key���� min���ٴ� ���ƾ� �ϰ� sibling���� min���� ���� key���� �־����. �װ� �ȵǸ� merge�ؾ��� �̤�
		//���ʿ��� �������� ������ . .
		//parent : merge�ؼ� ���ܳ� parent
		//target : merge�ؼ� ����ģ child
		
		//sibling�� key���� ������� Ȯ��
		target = parent; //merge�ؼ� ���ܳ� parent
		parent = parent.FindParentNode(this); //parent�� ���� parent�� ����
		int key  = 0;
		if(target.num == 0)
		{
			key = target.ptr.pair.getFirst().key;
		}
		else
		{
			key = target.pair.getFirst().key;
		}
		Node sibling = null;
		
		int numofkey = 0;
		if(key < parent.pair.getFirst().key)
		{
			if(parent.num != 1)
			{
				sibling = parent.pair.get(1).ptr;
			}
			else
			{
				sibling = parent.ptr;
			}
			numofkey = sibling.num;
		}
		else if(key >= parent.pair.getLast().key)
		{
			sibling = parent.pair.getLast().ptr;
			numofkey = sibling.num;
		}
		else
		{
			for(int i = 0; i < parent.num -1; i++)
			{
				if(key >= parent.pair.get(i).key && key < parent.pair.get(i+1).key)
				{
					sibling = parent.pair.get(i).ptr;
					numofkey = sibling.num;
				}
			}
		}

		
		//if(FindParentNode(key).num >= 1 && !underflow) //parent���� key borrow
		if(parent.num >= 1 && numofkey > minKeyNum)
		{
			if(key < parent.pair.getFirst().key)
			{
				//numofkey = parent.pair.get(1).ptr.num;
				Pair first = parent.pair.getFirst();
				//Node temp = parent.pair.get(1).ptr; //sibling node
				//target�� child�� �̹� merge�Ǿ��ٰ� ����
				//target�� pair�� null�� ���� ����..
				//merge�� node�� target�� RightChild��.
				
				//target���� key�� �̹� �����Ȼ���.
				//key borrow
				target.pair.addLast(new Pair(first.key, target.ptr));
				target.num += 1;
				//parent�� key �ٲٱ�
				parent.pair.getFirst().key = sibling.pair.getFirst().key;
				//parent.pair.removeFirst();
				//parent.pair.addFirst(new Pair(sibling.pair.getFirst().key, target));
				//sibling�� �ڽ��� �Ű��ֱ�
				target.ptr = sibling.pair.getFirst().ptr;
				//sibling���� pair����
				sibling.pair.removeFirst();
				sibling.num -= 1;
			}
			else if(key >= parent.pair.getLast().key)
			{
				//Node temp = parent.pair.getLast().ptr;
				//key borrow
				target.pair.addFirst(new Pair(parent.pair.getLast().key, sibling.ptr));
				target.num += 1;
				//parent�� key�ٲٱ�
				parent.pair.getLast().key = sibling.pair.getLast().key;
				//parent.pair.removeLast();
				//parent.pair.addLast(new Pair(sibling.pair.getLast().key, sibling));
				//sibling�� �����ͼ���
				
				//sibling.pair.getLast().ptr.ptr = sibling.ptr.ptr;
				
				sibling.ptr = sibling.pair.getLast().ptr;
				//sibling���� pair����
				sibling.pair.removeLast();
				sibling.num -= 1;
			}
			else
			{
				for(int i = 0; i < parent.num -1; i++)
				{
					if(key >= parent.pair.get(i).key && key < parent.pair.get(i+1).key)
					{
						sibling = parent.pair.get(i).ptr;
						//key borrow
						target.pair.addFirst(new Pair(parent.pair.get(i).key, sibling.ptr));
						target.num += 1;
						//parent�� key ����
						parent.pair.get(i).key = sibling.pair.getLast().key;
						//parent.pair.remove(i);
						//parent.pair.add(i, new Pair(sibling.pair.getLast().key, sibling));
						//sibling�� �����Ϳ���
						
						//sibling.pair.getLast().ptr.ptr = sibling.ptr.ptr;
						
						sibling.ptr = sibling.pair.getLast().ptr;
						//sibling���� pair ����
						sibling.pair.removeLast();
						sibling.num -= 1;
					}
				}
			}
			/**************�߰��Ⱥκ�****************/
//			if(parent.num != 0)
//			{
//				for(int i = 0; i < parent.num-1; i++)
//				{
//					parent.pair.get(i).key = parent.pair.get(i+1).ptr.pair.getFirst().key;
//				}
//				parent.pair.getLast().key = parent.ptr.pair.getFirst().key;
//			}
//			
		}
		//2. merge
		else
		{
//			System.out.println("merge in non leaf node");
			parent = target.MergeInNonLeafNode(this, parent, key, degree);
		}
		
		if(!parent.root && parent.num < minKeyNum)
		{
			parent = UnderflowInNonLeafNode(parent, sibling, minKeyNum);
		}
		
		return parent;
	}

		
		
		
	public Node MergeInNonLeafNode(Node root, Node parent, int key, int degree)
	{
//		System.out.println("in Merge In Non Leaf Node");
		//this == target
		//merge�ϰ� parent�� ��ȯ. merge�ϸ� this�� ��ȯ�ϴ°Ŷ� ���� ȿ���� �ɵ�. root�� key�� �ϳ� �϶� root�� merge�ϸ� root�� height -1
		//parent�� �ִ� key�� ����..
		//�������� merge
		Node sibling = null; //merge�� sibling node
 		if(key < parent.pair.getFirst().key)
		{
 			if(parent.num == 1)
				sibling = parent.ptr;
 			else
 				sibling = parent.pair.get(1).ptr;
			//������ sibling�� merge.
			sibling.pair.addFirst(new Pair(parent.pair.getFirst().key, this.ptr));
			sibling.num += 1;
			for(int i = this.num -1; i >= 0; i--)
			{
				sibling.pair.addFirst(this.pair.get(i));
				sibling.num += 1;
			}
			
			//���࿡ parent�� ���� root���ٸ�
			if(parent.root && parent.num == 1)
			{
				sibling.root = true;
			}
			parent.pair.removeFirst();
			parent.num -= 1;
			
			if(sibling.root == true)
			{
				parent = sibling;
			}
			
			//
		}
		else //���� sibling�� merge
		{
			int index = 0;
			if(key >= parent.pair.getLast().key)
			{
				sibling = parent.pair.getLast().ptr;
				index = parent.num -1;
			}
			else
			{
				for(int i = 0; i < parent.num-1; i++)
				{
					if(key >= parent.pair.get(i).key && key < parent.pair.get(i+1).key)
					{
						sibling = parent.pair.get(i).ptr;
						index = i;
					}
				}
			}
			
			sibling.pair.addLast(new Pair(parent.pair.get(index).key, sibling.ptr));
			sibling.num += 1;
			for(int i = 0; i < this.num; i++)
			{
				sibling.pair.addLast(this.pair.get(i));
				sibling.num += 1;
			}
			sibling.ptr = this.ptr;
			
			if(parent.root && parent.num == 1)
			{
				sibling.root = true;
			}
			
			//error�ߴ� �κ� 
			if(parent.num == 1 || index == parent.num -1)
				parent.ptr = parent.pair.getLast().ptr;
			else
			{
				parent.pair.get(index+1).ptr = parent.pair.get(index).ptr;
			}
			
			parent.pair.remove(index);
			parent.num -= 1;
			
			if(sibling.root == true)
			{
				parent = sibling;
			}
		}
 		
		/**************�߰��Ⱥκ�****************/
//		if(parent.num != 0)
//		{
//			for(int i = 0; i < parent.num-1; i++)
//			{
//				parent.pair.get(i).key = parent.pair.get(i+1).ptr.pair.getFirst().key;
//			}
//			parent.pair.getLast().key = parent.ptr.pair.getFirst().key;
//		}
		
		
		//sibling�� �����Ŵϱ� target�� sibling���� �ٲ����
		if(sibling.num >= degree)
		{
			parent = sibling.SplitNode(root, degree);
		}
		
		return parent;
		
	}
	
	public Node MergeInLeafNode(Node root, Node parent, int key, int degree)
	{
//		System.out.println("in Merge In Leaf Node");
		//this == target
		//merge�ϰ� merge�� node�� parent ��ȯ
		Node sibling = null;
		int tempkey = key;
		if(this.num != 0 && this.pair.getFirst().key != key)
			tempkey = this.pair.getFirst().key;
		
		if(parent.pair.getLast().key <= tempkey) //������ parent�� ptr�� �� ����̶� .. ���������� merge�� <= �̷��� �ȵ����������� �������� merge�ؼ� �ٽ� ���������� ��������
		{
			sibling = parent.pair.getLast().ptr;
			for(int i = 0; i < this.num; i++)
			{
				sibling.pair.addLast(this.pair.get(i));
				sibling.num += 1;
			}
			sibling.ptr = this.ptr;
			parent.ptr = sibling;
			parent.pair.removeLast();
			parent.num -= 1;
			
			
			if(sibling.num >= degree)
			{
				parent = sibling.SplitNode(root, degree);
			}
		}
		
		else if(tempkey < parent.pair.getFirst().key)
		{
			sibling = this.ptr;
			
			for(int i = 0; i < sibling.num; i++)
			{
				this.pair.addLast(sibling.pair.get(i));
				this.num += 1;
			}
			this.ptr = sibling.ptr;
			
			/****************�����Ⱥκ�************/
			if(parent.num <= 1)
			{
				parent.ptr = this;
			}
			else
			{
				parent.pair.get(1).ptr = this;
			}
			parent.pair.removeFirst();
			parent.num -= 1;
			
			if(this.num >= degree)
			{
				parent = this.SplitNode(root, degree);
			}

		}
		else//ù��°�� �߰��� �ִ� child�� ���, ������ node�� merge�Ѵ�. <= �̷����� �ȵ����������������������� �ϴ°� ������ �־���
		{
			//merge
//			int splitkey = this.pair.getFirst().key;
			int splitkey = key;
			if(this.num != 0)
			{
				int a = this.pair.getFirst().key;
				if(a < splitkey)
					splitkey = a;
			}
			int index = 0;
			for(int i = 0; i < parent.num; i++)
			{
				if(parent.pair.get(i).key == splitkey)
				{
					sibling = parent.pair.get(i).ptr;
					index  = i;
				}
			}
			
			for(int i = 0; i < this.num; i++)
			{
				sibling.pair.addLast(this.pair.get(i));
				sibling.num += 1;
			}
			sibling.ptr = this.ptr;
			
			parent.pair.get(index+1).ptr = sibling;
			parent.pair.remove(index);
			parent.num -= 1;
			
			if(sibling.num >= degree)
			{
				parent = sibling.SplitNode(root, degree);
			}
			
		}
		
		/**************�߰��Ⱥκ�****************/
		if(parent.num != 0)
		{
			for(int i = 0; i < parent.num-1; i++)
			{
				parent.pair.get(i).key = parent.pair.get(i+1).ptr.pair.getFirst().key;
			}
			parent.pair.getLast().key = parent.ptr.pair.getFirst().key;
		}
		
		return parent;
	}
	
	
	
	
	public Node FindParentNode(Node root)
	{
		//this == target
//		System.out.println("in FINDPARENTNODE");
		Node temp = root;
	
		
		int key;
		if(this.num == 0)
		{
			key = this.ptr.pair.getFirst().key;
		}
		else
		{
			key = this.pair.getFirst().key;
		}
		
		while(temp.height > this.height)
		{
			if(key < temp.pair.getFirst().key)
			{
				if(temp.pair.getFirst().ptr == this)
					return temp;
				temp = temp.pair.getFirst().ptr;
			}
			else if (key >=  temp.pair.getLast().key)
			{
				if(temp.ptr == this)
					return temp;
				temp = temp.ptr;
			}
			else
			{
				for(int i = 0; i < temp.num - 1; i++)
				{
					if(temp.pair.get(i).key <= key && temp.pair.get(i+1).key > key)
					{
						if(temp.pair.get(i+1).ptr == this)
							return temp;
						temp = temp.pair.get(i+1).ptr;
					}
				}
			}
		}
		
//		System.out.println("FP error");
		return null;
		
	}
	
	
	

	
	public void SingleSearch(String file, int num)
	{
		//��±��� ����
		try
		{
			//PrintWriter outStream = new PrintWriter(new FileOutputStream(file, true));
			Scanner inStream = new Scanner(new FileInputStream(file));
			int degree = inStream.nextInt();
			Node.degree = degree;

			
			while(inStream.hasNext())
			{
				String str = inStream.next();
				String[] pair = str.split(",");
				InsertKey(Node.degree, file, Integer.parseInt(pair[0]), Integer.parseInt(pair[1]));
//				outStream.println(pair[0] +","+pair[1]);
			}
			inStream.close();
			
			SearchKey(num);
//			outStream.println("finish Searching key " + num);
			//outStream.close();
			
		} catch (FileNotFoundException e) 
		{
//			System.out.println("single search error");
		}
	}
	
	public void SearchKey(int key)
	{
		//root�� ȣ���� ����. this == root
		
		Node temp = this;
		if(!leaf)
		{
			for(int i = 0; i < temp.num-1; i++)
			{
				System.out.print(temp.pair.get(i).key + ",");
			}
			System.out.println(temp.pair.getLast().key);
			
			//1. index���� key���� ���� ��� -> ���� index�� ptr�� ����.
			//2. �ּ� index���� key�� ����
			//3. �ִ� index���� key�� ŭ
			//4. key�� ���� �� index�� ������

			if(temp.pair.getLast().key <= key)
			{
				temp = temp.ptr;
				temp.SearchKey(key);
			}
			else if(temp.pair.getFirst().key > key)
			{
				temp = temp.pair.getFirst().ptr;
				temp.SearchKey(key);
			}
			else
			{
				for(int i = 0; i < temp.num-1; i++)
				{
					if(temp.pair.get(i).key <= key && temp.pair.get(i+1).key > key)
					{
						temp = temp.pair.get(i+1).ptr;
						temp.SearchKey(key);
						break;
					}
				}
			}
			
		}
		
		else
		{
			int index = -1;
			for(int i = 0; i < this.num; i++)
			{
				if(this.pair.get(i).key == key)
				{
					System.out.println(this.pair.get(i).value);
					index = i;
				}
			}
			
			if(index == -1)
				System.out.println("NOT FOUND");
		}
			
	}
	
	public void RangeSearch(String file, int num1, int num2)
	{
		//��±��� ����
		try
		{
			Scanner inStream = new Scanner(new FileInputStream(file));
			int degree = inStream.nextInt();
			Node.degree = degree;
			
//			while(!inStream.next().equalsIgnoreCase("Delete"))
//			{
//				inStream.next();
//			}
//			inStream.hasNext(); //"delete"
			
			while(inStream.hasNext())
			{
				String str = inStream.next();
				String[] pair = str.split(",");
				InsertKey(Node.degree, file, Integer.parseInt(pair[0]), Integer.parseInt(pair[1]));
//				outStream.println(pair[0] +","+pair[1]);
			}
			inStream.close();
			
			RSearchKey(num1, num2);

		} catch (FileNotFoundException e) 
		{
//			System.out.println("range search error");
		}
	}
	
	
	public void RSearchKey(int key1, int key2)
	{
		//root == this
		 Node temp = this;
		 while(temp.height != 0)
		 {
			if(temp.pair.getLast().key <= key1)
			{
				temp = temp.ptr;
			}
			else if(temp.pair.getFirst().key > key1)
			{
				temp = temp.pair.getFirst().ptr;
			}
			else
			{
				for(int i = 0; i < temp.num-1; i++)
				{
					if(temp.pair.get(i).key <= key1 && temp.pair.get(i+1).key > key1)
					{
						temp = temp.pair.get(i+1).ptr;
					}
				}
			}
		 }
		 //temp�� leaf node�� ��
//		 System.out.println(temp.pair.getFirst().key);
		 
		 while(temp != null && temp.pair.getFirst().key <= key2 /*| temp.pair.getFirst().key == key2)*/)
		 {
			 for(int i = 0; i < temp.num; i++)
			 {
				 if(temp.pair.get(i).key >= key1 && temp.pair.get(i).key <= key2)
				 {
					 System.out.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
				 }
			 }
			 temp = temp.ptr;
		 }
		  
	}
	
	
	
	public void InsertKey(int degree, String file, int key, int value) //throws FileNotFoundException
	{
//		PrintWriter outStream = new PrintWriter(new FileOutputStream(file,true));
//		outStream.println("\n");
//		System.out.println("in INSERTKEY");
		//�ߺ� �˻�
		if(this.num != 0)
		{
			Node look = this;
			while(!look.leaf)
			{
				look = look.pair.getFirst().ptr;
			}
		
			while(look != null)
			{
				for(int i = 0; i < look.num; i++)
					if(look.pair.get(i).key == key)
						return;
				look = look.ptr;
			}
		}
		
		if(num == 0)
		{
//			System.out.println("in if");
			
			leaf = true;
			root = true;
			Pair pair = new Pair(key, value);
			this.pair.add(pair);
			num = 1;
//			outStream.println(key + " is in ROOT NODE");
			height = 0;
		}
		else
		{
//			System.out.println("in else");
			
			Node target = FindLeafNode(key);
//			outStream.println("Find leaf node for KEY " + key + "...");
			
			if(key < target.pair.get(0).key)
			{
//				outStream.println("KEY " + key + " is inserted in front of KEY " + target.pair.get(0).key);
				target.pair.addFirst(new Pair(key, value));
				
			}
			else if (key > target.pair.getLast().key)
			{
//				outStream.println("KEY " + key + " is inserted behind KEY " + target.pair.getLast().key);
				target.pair.addLast(new Pair(key, value));
			}
			else
			{
				for(int i = 0; i < target.num - 1; i++)
				{
					if(key > target.pair.get(i).key && key < target.pair.get(i+1).key)
					{
//						outStream.println("KEY " + key + " is inserted between KEY " + target.pair.get(i).key + " and KEY " + target.pair.get(i+1).key);
						target.pair.add(i+1, new Pair(key, value));
					}
				}
			}
			target.num += 1;
			
			
			if(target.num == degree)
			{
//				outStream.println("\nSPLIT node ...");
				target = target.SplitNode(this, degree); //split�ؼ� ���� subtree�� root�� return.\
				
				if(target.root)
				{
					this.root = true;
					this.leaf = false;
					this.num = target.num;
					this.pair = target.pair;
					this.ptr = target.ptr;
					this.height = target.height;
				}
				
//				outStream.println("Height is : " + target.height);
//				outStream.print("Parent key : ");
//				for(int i = 0; i < target.num; i++)
//				{
//					outStream.print(target.pair.get(i).key +" " );
//				}
//				outStream.print("\nChild key : ");
//				
//				for(int i = 0; i < target.num; i++)
//				{
//					for(int j = 0; j < target.pair.get(i).ptr.num; j++)
//					{
//						outStream.print(target.pair.get(i).ptr.pair.get(j).key + " ");
//					}
//					outStream.print("| ");
//				}
//				for(int i = 0; i < target.ptr.num; i++)
//				{
//					outStream.print(target.ptr.pair.get(i).key +" " );
//				}
			}
		}
//		System.out.println("Insert " + key + " finish");

//		Node temp = this;
//		while(!temp.leaf)
//		{
//			temp = temp.pair.getFirst().ptr;
//		}
//		PrintWriter outStream = new PrintWriter(new FileOutputStream(file,true));
		//outStream.println("Insert");
//		while(temp != null)
//		{
//			for(int i = 0; i < temp.num; i++)
//				//outStream.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
//				System.out.println(temp.pair.get(i).key+","+temp.pair.get(i).value);
//			temp = temp.ptr;
//		}
//		outStream.close();
	}
	
	public Node ArrangeNode(Node root, Node origin, int degree) //root�� tree�� root�� node�� split�ؼ� ���� subtree�� root��
	{
//		System.out.println("in ARRANGENODE");
			
		int tempkey = 0;
		Node trial = this;
		for(int i = 0; i < this.height; i++)
		{
			trial = trial.pair.getFirst().ptr;
		}
		tempkey = trial.pair.getFirst().key;
		Node target = FindNonLeafNode(root, tempkey); //���� subtree���� keyã��, !!�� node�� root���?!!
		
		//Node target = target.pair.getFirst().ptr.FindParentNode(root);
		
		//parent�� ã���ǰ�? ����;
		
		//�ᱹ target�� splitnode�� ������ node�� ã�ڴٴ°��ݽ�
			
//			if(target == null)
//			{
//				if(root.height == this.height) //���� ������� subtree�� ���̰� root�� ���ٴ°� root node�� �߰��ؾ��Ѵٴ� ��
//				{
//					target = root;
//				}
//				else //�̰� ���ذ� �ȵ�;; ���� ���µ� �� ���ظ� ���ϴ� ������; �� ���� �˰ٴ� �׶� �������ߴ���
//					//�� ������ leaf node�� parent�� ã�� �;����ǵ�......
//				{
//					Node temp = root;
//					for(int i = 0; i < root.height-1; i++)
//					{
//						temp = temp.pair.getFirst().ptr;
//					}
//					target = temp;
//				}
//			}
//			else
//			{
//				if(root.height == this.height) //���� ������� subtree�� ���̰� root�� ���ٴ°� root node�� �߰��ؾ��Ѵٴ� ��
//				{
//					target = root;
//				}
//				
//				if(target.height - this.pair.getFirst().ptr.height > 1)
//				{
//					//tempkey�� root�� ���°���� �˾Ƴ���
//					//height ���ؼ� targetã��
//					int index = 0;
//					for(int i = 0; i < target.num; i++)
//					{
//						if(target.pair.get(i).key == tempkey)
//						{
//							index = i;
//						}
//					}
//					
//					Node first = null;
//					
//					if(tempkey == target.pair.getLast().key)
//					{
//						first = target.ptr;
//						for(int i = 0; i < target.height -2; i++)
//						{
//							first = target.pair.getFirst().ptr;
//						}
//					}
//					else
//					{
//						first = target.pair.get(index+1).ptr;
//						for(int i = 0; i < target.height -2; i++)
//						{
//							first = target.pair.getFirst().ptr;
//						}
//					}
//					
//					target = first;
//				}
//				
//			}
		target = origin.FindParentNode(root);

			//node ���ο��� key�� ��Һ񱳸� �ؼ� �ǵڿ� ���ԵǴ°Ŷ� �ƴ� ��� ���������. right subtree����..
			
			int newkey = this.pair.getFirst().key;
			if(newkey > target.pair.getLast().key)
			{ 
				//�� ���� node ���̱�
				boolean check = target.pair.getLast().ptr.leaf;
				
				if(target.pair.getLast().ptr.leaf) //target�� subtree�� root�ϰ�� �����͸� �� �̾���� ��.
					target.pair.getLast().ptr.ptr = this.pair.getFirst().ptr;
				target.pair.addLast(this.pair.getFirst());
				target.ptr = this.ptr;
				
				
				
				if(check)
				{
					target.pair.getLast().ptr.leaf = true;
					target.pair.getLast().ptr.root = false;
					target.ptr.leaf = true;
					target.ptr.root = false;
				}
			
			}
			else if(newkey < target.pair.getFirst().key)
			{
				//�� �տ� ���ԵǴ� ��� �����͸� �ٲ�����ϴ��� ....
				//Node node1 = root.FindLeafNode(tempkey-1);
			
				int store = target.pair.getFirst().key;
				target.pair.remove(0);
				target.pair.addFirst(this.pair.getFirst());
				target.pair.add(1, new Pair(store, this.ptr));
		
			
			}
			else
			{
				for(int i = 0; i < target.num-1; i++)
				{
					if(newkey > target.pair.get(i).key && newkey < target.pair.get(i+1).key)
					{
						boolean check = target.pair.get(i).ptr.leaf;
						
						int nextkey = target.pair.get(i+1).key;
						target.pair.remove(i+1);
						target.pair.add(i+1, this.pair.getFirst()); //���� ���ܵ� node�� �� ���� node����ȵȻ���
						target.pair.add(i+2, new Pair(nextkey, this.ptr));
						
					}
				}
			}
			target.num += 1;
			
			if(target.num == degree)
			{
				target = target.SplitNode(root, degree);
			}
			
			if(target.root) //target�� root���
			{
				root = target;
				return root;
			}
			
	
		
	
		return target;
	}
	
	
	
	public Node FindLeafNode(int key) //leaf node ã��
	{
//		System.out.println("in FINDLEAFNODE");
		Node temp = null;
		
		if(!leaf)
		{
			for(int i = 0; i < num - 1; i++)
			{
				if(pair.get(i).key <= key && pair.get(i+1).key > key)
				{
					temp = this.pair.get(i+1).ptr;
					temp = temp.FindLeafNode(key);
				}
			}
			if(pair.get(0).key > key)
			{
				temp = this.pair.get(0).ptr;
				temp = temp.FindLeafNode(key);
			}
			if(pair.get(num - 1).key <= key)
			{
				temp = this.ptr;
				temp = temp.FindLeafNode(key);
			}
		}
		
		else
			temp = this;
		return temp;
	}
	
	
	
	public Node SplitNode(Node rootnode, int degree) 
	{
		Node temp = new Node();
		int mid =  degree/2;
		
		Pair midpair = this.pair.get(mid);
		
		Node LeftChild = new Node();
		Node RightChild = new Node();
		LeftChild.pair = new LinkedList<Pair>();
		RightChild.pair = new LinkedList<Pair>();
		
		for(int i = 0; i < this.num; i++)
		{
			Pair ptr = this.pair.get(i);
			if(i < mid)
			{
				LeftChild.pair.add(ptr);
				LeftChild.num += 1;
				
			}
			else if (i == mid)
			{
				continue;
			}
			else //(i > mid)
			{
				RightChild.pair.add(ptr);
				RightChild.num += 1;
			}
		}
		
		if(midpair.ptr != null)
		{
			LeftChild.ptr = midpair.ptr;
		}
		
		if(this.ptr != null)
		{
			RightChild.ptr = this.ptr;
		}
	
		if(this.root)
		{
			int rootkey = midpair.key;
			LeftChild.ptr = midpair.ptr;
			RightChild.ptr = this.ptr;
			temp.pair.add(new Pair(rootkey, LeftChild));
			temp.ptr = RightChild;
			
			
			temp.root = true;
			temp.num = 1;
			temp.leaf = false;
			
			if(this.leaf) //ó������ �ڽĳ�忴���Ƿ� split�Ŀ��� ���� child�� ������ child�� leaf node�� ��
			{
				LeftChild.leaf = RightChild.leaf = true;
				LeftChild.ptr = RightChild;
				RightChild.ptr = this.ptr;
				RightChild.pair.addFirst(midpair);
				RightChild.num += 1;
			}
			
		}
		else
		{
			if(this.leaf) //ó������ �ڽĳ�忴���Ƿ� split�Ŀ��� ���� child�� ������ child�� leaf node�� ��
			{
				LeftChild.leaf = RightChild.leaf = true;
				LeftChild.ptr = RightChild;
				RightChild.ptr = this.ptr;
				RightChild.pair.addFirst(midpair);
				RightChild.num += 1;
				
				//pointer�߰�
				Node parent = FindNonLeafNode(rootnode, LeftChild.pair.getFirst().key);
				if(parent != null)  //Non leaf�߿� key�� �����ϴ� node�� ����
				{
					Node link = rootnode.FindLeafNode((LeftChild.pair.getFirst().key)-1);
					link.ptr = LeftChild;
				}
				
				/*
				int compare = LeftChild.pair.getFirst().key;
				if(compare >= rootnode.pair.getFirst().key) //root�� ù��° subtree�� �ƴ�
				{
					Node link = rootnode.FindLeafNode(compare-1);
					link.ptr = LeftChild;
				}
				*/
				
			}
			else //�߰��� ���� ���
			{
				LeftChild.leaf = RightChild.leaf = false;
			}
		
			temp.leaf = false;
			temp.num = 1;
			temp.pair.add(new Pair(midpair.key, LeftChild));
			temp.ptr = RightChild;
			
			
			
			
		}
		//���� �߰��κ�
		RightChild.height = this.height;
		LeftChild.height = this.height;
		temp.height = this.height +1;
		
		//split ��
		
		//arrange ��ġ��
		if(!temp.root)
		{
			temp = temp.ArrangeNode(rootnode, this, degree); //���� B+ tree�� ��ġ��
		}
		return temp;
		
//		Node temp = new Node();
//		int mid =  degree/2;
//		
//		Pair midpair = this.pair.get(mid);
//		
////		Node LeftChild = new Node();
//		Node RightChild = new Node();
////		LeftChild.pair = new LinkedList<Pair>();
//		RightChild.pair = new LinkedList<Pair>();
//		
//		int num = 0;
//		for(int i = 0; i < this.num; i++)
//		{
//			Pair ptr = this.pair.get(i);
//			if(i < mid)
//			{
////				LeftChild.pair.add(ptr);
////				LeftChild.num += 1;
//				continue;
//			}
//			else if (i == mid)
//			{
//				num += 1;
//				continue;
//			}
//			else //(i > mid)
//			{
//				RightChild.pair.add(ptr);
//				RightChild.num += 1;
//				num += 1;
//				//this.pair.removeLast();
//				//this.num -= 1;
//			}
//		}
//		
//		for(int i = 0; i < num; i++)
//		{
//			this.pair.removeLast();
//			this.num -= 1;
//		}
//		
//		if(this.ptr != null)
//		{
//			RightChild.ptr = this.ptr;
//		}
//		
//		if(midpair.ptr != null)
//		{
////			LeftChild.ptr = midpair.ptr;
//			this.ptr = midpair.ptr;
//		}
//	
//		if(this.root)
//		{
//			int rootkey = midpair.key;
////			LeftChild.ptr = midpair.ptr;
////			RightChild.ptr = this.ptr;
////			temp.pair.add(new Pair(rootkey, LeftChild));
//			temp.pair.add(new Pair(rootkey, this));
//			temp.ptr = RightChild;
//			
//			
//			temp.root = true;
//			temp.num = 1;
//			temp.leaf = false;
//			
//			if(this.leaf) //ó������ �ڽĳ�忴���Ƿ� split�Ŀ��� ���� child�� ������ child�� leaf node�� ��
//			{
////				LeftChild.leaf = RightChild.leaf = true;
////				LeftChild.ptr = RightChild;
//				this.leaf = RightChild.leaf = true;
//				this.ptr = RightChild;
////				RightChild.ptr = this.ptr;
//				RightChild.pair.addFirst(midpair);
//				RightChild.num += 1;
//			}
//			
//		}
//		else
//		{
//			if(this.leaf) //ó������ �ڽĳ�忴���Ƿ� split�Ŀ��� ���� child�� ������ child�� leaf node�� ��
//			{
////				LeftChild.leaf = RightChild.leaf = true;
////				LeftChild.ptr = RightChild;
//				this.leaf = RightChild.leaf = true;
//				this.ptr = RightChild;
////				RightChild.ptr = this.ptr;
//				RightChild.pair.addFirst(midpair);
//				RightChild.num += 1;
//				
//				//pointer�߰�, target�� ��Ȱ���ϸ� ���� leaf�� �����͸� �ٽ� �̾��� �ʿ䰡 ����...
////				Node parent = FindNonLeafNode(rootnode, LeftChild.pair.getFirst().key);
////				if(parent != null)  //Non leaf�߿� key�� �����ϴ� node�� ����
////				{
////					Node link = rootnode.FindLeafNode((LeftChild.pair.getFirst().key)-1);
////					link.ptr = LeftChild;
////				}
//				
//			}
//			else //�߰��� ���� ���
//			{
////				LeftChild.leaf = RightChild.leaf = false;
//				this.leaf = RightChild.leaf = false;
//			}
//		
//			temp.leaf = false;
//			temp.num = 1;
////			temp.pair.add(new Pair(midpair.key, LeftChild));
//			temp.pair.add(new Pair(midpair.key, this));
//			temp.ptr = RightChild;
//			
//			
//			
//			
//		}
//		//���� �߰��κ�
//		RightChild.height = this.height;
////		LeftChild.height = this.height;
//		temp.height = this.height +1;
//		
//		//split ��
//		
//		//arrange ��ġ��
//		if(!temp.root)
//		{
//			temp = temp.ArrangeNode(rootnode, degree); //���� B+ tree�� ��ġ��
//		}
//		return temp;
	}
	
	
	public Node FindNonLeafNode(Node node, int key) //�����غ��ϱ� Pair���� Node�� ��ȯ�ؾ� ����Լ��� ������ �� ���Ƽ� �ڵ带 �ٽ� ¥���� ��.
	{
//		System.out.println("in FINDNONLEAFNODE");
//		System.out.println("key is " + key);
		
		

		if(!node.leaf)
		{
			for(int i = 0; i < node.num; i++)
			{
				if(node.pair.get(i).key == key)
				{
					return node;
				}
			}
			
			if(node.pair.getFirst().key > key)
			{
//				System.out.println("case1");
				return FindNonLeafNode(node.pair.getFirst().ptr, key);
			}
			else if(node.pair.getLast().key < key)
			{
//				System.out.println("case1");
				return FindNonLeafNode(node.ptr, key);
			}
			else
			{
//				System.out.println("case3");
				for(int i = 0; i < node.num-1; i++)
				{
					if(node.pair.get(i).key < key && node.pair.get(i+1).key > key)
						return FindNonLeafNode(node.pair.get(i+1).ptr, key);
				}
				
			}
		}
//		System.out.println("error");
	
		return null;
	}
	

}
