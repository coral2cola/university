import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.LinkedList;
import java.util.Scanner;

//Bptree의 Node

public class Node {
	
	static int degree = 0;
	int num = 0;
	LinkedList<Pair> pair = new LinkedList<Pair>();
	Node ptr = null;
	boolean leaf;
	boolean root = false;
	int height = 0; //leaf가 0, leaf로부터의  높이를 나타냄.
	
	
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
			
			Scanner inStream = new Scanner(new FileInputStream(data)); //프로젝트 폴더 안에 file이 존재하고 있음.
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
		
 		int minKeyNum = (degree-1)/2; //node안에 있는 key의 개수가 이 값보다 적어지면 underflow가 발생함.
		
		Node target = FindLeafNode(key); //key가 들어있는 leaf node를 찾음
		Node parent = target.FindParentNode(this);
		//일단 key를 지워야함
		int num = 0;
		for(int i = 0; i < target.num; i++)
		{
			if(target.pair.get(i).key == key)
			{
				num = i;
			}
		}
		
		/******************추가된 부분********************/
		/******이 부분은 index node만 수정함*****************/
		Node parentnode = target.FindNonLeafNode(this,key); //위에 있을지도 모르는 parent
		Node firstparent = target.FindParentNode(this); //높이가 1인 parent
		
		//높이가 1인 parent의 가장 오른쪽 leaf node의 first key가 index node의 key가 된다
		//tree에서 가장 오른쪽에 있는 leaf node의 first key는 index node에 존재하지 않기 때문에 parentnode가 null이 될 수 있다
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
		
		//borrow도 merge도 일어나지 않는데 index node에서 key가 바뀌어야하는 경우
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
//		if(num == 0 && parentnode != null /*&& target.num != 1*/) //non leaf node의 index 바꿔주기
//		{
//			//Node parentnode = target.FindNonLeafNode(this, key);
//			//삭제할 key가 index node에 있으면 key를 바꿔줌
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
//							if(parentnode.pair.get(i+1).key != key) //상당히 문제가 복잡해져버림..근데 그대로 두면 merge할때 대소비교가 안됨
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
		
		target.pair.remove(num); //key값이 같은 pair를 발견하면 삭제.
		target.num -= 1;
		
		
		//Node parent = target.FindParentNode(key);
		//Node parent = target.FindParentNode(this);
		if(target.num < minKeyNum) //underflow 발생
		{
			//if(target.ptr != null && target.FindParentNode(this) == target.)
			if(target.ptr != null && parent == target.ptr.FindParentNode(this) && target.ptr.num > minKeyNum)
			//if(FindParentNode(key) == FindParentNode(target.ptr.pair.getFirst().key) && target.ptr.num >= minKeyNum) //right sibling에서 key borrow
			{
//				System.out.println("borrow key from sibling");
				Node sibling = target.ptr;
				Pair borrow = sibling.pair.getFirst();
				sibling.pair.removeFirst();
				sibling.num -= 1;
				target.pair.addLast(borrow);
				target.num += 1;
					
				//rootnode 수정
				
				/******************추가된 부분********************/
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
//						count = i; //같은 숫자가 parent안에 존재한다면 갱신해서 맨 마지막 숫자를 선택하게 됨.
//					}
//				}
//				parent.pair.get(count).key = sibling.pair.getFirst().key;
//				
//				/******************추가된 부분********************/
//				if(count != 0)
//				{
//					parent.pair.get(count-1).key = parent.pair.get(count).ptr.pair.getFirst().key;
//				}
				
			}
			//2. merge
			else
			{
//				System.out.println("merge in leaf node");
				//right child랑 merge하고 split key 삭제
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
		
			
			//leafnode 단위에서 underflow에 대한 작업을 수행했는데도 underflow가 발생한다면
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
		//1. parent한테서 빌려옴
		//parent의 key값이 min보다는 많아야 하고 sibling한테 min보다 많은 key값이 있어야함. 그게 안되면 merge해야함 ㅜㅜ
		//왼쪽에서 빌려오는 것으로 . .
		//parent : merge해서 생겨난 parent
		//target : merge해서 합쳐친 child
		
		//sibling의 key값이 충분한지 확인
		target = parent; //merge해서 생겨난 parent
		parent = parent.FindParentNode(this); //parent의 상위 parent를 구함
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

		
		//if(FindParentNode(key).num >= 1 && !underflow) //parent에서 key borrow
		if(parent.num >= 1 && numofkey > minKeyNum)
		{
			if(key < parent.pair.getFirst().key)
			{
				//numofkey = parent.pair.get(1).ptr.num;
				Pair first = parent.pair.getFirst();
				//Node temp = parent.pair.get(1).ptr; //sibling node
				//target의 child는 이미 merge되었다고 가정
				//target의 pair은 null일 수도 있음..
				//merge한 node는 target의 RightChild임.
				
				//target에서 key는 이미 삭제된상태.
				//key borrow
				target.pair.addLast(new Pair(first.key, target.ptr));
				target.num += 1;
				//parent의 key 바꾸기
				parent.pair.getFirst().key = sibling.pair.getFirst().key;
				//parent.pair.removeFirst();
				//parent.pair.addFirst(new Pair(sibling.pair.getFirst().key, target));
				//sibling의 자식을 옮겨주기
				target.ptr = sibling.pair.getFirst().ptr;
				//sibling에서 pair제거
				sibling.pair.removeFirst();
				sibling.num -= 1;
			}
			else if(key >= parent.pair.getLast().key)
			{
				//Node temp = parent.pair.getLast().ptr;
				//key borrow
				target.pair.addFirst(new Pair(parent.pair.getLast().key, sibling.ptr));
				target.num += 1;
				//parent의 key바꾸기
				parent.pair.getLast().key = sibling.pair.getLast().key;
				//parent.pair.removeLast();
				//parent.pair.addLast(new Pair(sibling.pair.getLast().key, sibling));
				//sibling의 포인터수정
				
				//sibling.pair.getLast().ptr.ptr = sibling.ptr.ptr;
				
				sibling.ptr = sibling.pair.getLast().ptr;
				//sibling에서 pair제거
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
						//parent의 key 수정
						parent.pair.get(i).key = sibling.pair.getLast().key;
						//parent.pair.remove(i);
						//parent.pair.add(i, new Pair(sibling.pair.getLast().key, sibling));
						//sibling과 포인터연결
						
						//sibling.pair.getLast().ptr.ptr = sibling.ptr.ptr;
						
						sibling.ptr = sibling.pair.getLast().ptr;
						//sibling에서 pair 제거
						sibling.pair.removeLast();
						sibling.num -= 1;
					}
				}
			}
			/**************추가된부분****************/
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
		//merge하고 parent를 반환. merge하면 this를 반환하는거랑 같은 효과가 될듯. root에 key가 하나 일때 root를 merge하면 root의 height -1
		//parent에 있는 key를 들고옴..
		//왼쪽으로 merge
		Node sibling = null; //merge할 sibling node
 		if(key < parent.pair.getFirst().key)
		{
 			if(parent.num == 1)
				sibling = parent.ptr;
 			else
 				sibling = parent.pair.get(1).ptr;
			//오른쪽 sibling과 merge.
			sibling.pair.addFirst(new Pair(parent.pair.getFirst().key, this.ptr));
			sibling.num += 1;
			for(int i = this.num -1; i >= 0; i--)
			{
				sibling.pair.addFirst(this.pair.get(i));
				sibling.num += 1;
			}
			
			//만약에 parent가 단일 root였다면
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
		else //왼쪽 sibling과 merge
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
			
			//error뜨는 부분 
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
 		
		/**************추가된부분****************/
//		if(parent.num != 0)
//		{
//			for(int i = 0; i < parent.num-1; i++)
//			{
//				parent.pair.get(i).key = parent.pair.get(i+1).ptr.pair.getFirst().key;
//			}
//			parent.pair.getLast().key = parent.ptr.pair.getFirst().key;
//		}
		
		
		//sibling에 넣은거니까 target을 sibling으로 바꿔줬긔
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
		//merge하고 merge된 node의 parent 반환
		Node sibling = null;
		int tempkey = key;
		if(this.num != 0 && this.pair.getFirst().key != key)
			tempkey = this.pair.getFirst().key;
		
		if(parent.pair.getLast().key <= tempkey) //어차피 parent의 ptr이 될 운명이라 .. 오른쪽으로 merge함 <= 이러면 안됐음ㅋㅋㅋㅋ 왼쪽으로 merge해서 다시 오른쪽으로 보내야함
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
			
			/****************수정된부분************/
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
		else//첫번째와 중간에 있는 child일 경우, 오른쪽 node와 merge한다. <= 이랬으면 안됐음ㅋㅋㅋㅋㅋㅋ왼쪽으로 하는건 이유가 있었어
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
		
		/**************추가된부분****************/
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
		//출력까지 구현
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
		//root가 호출할 예정. this == root
		
		Node temp = this;
		if(!leaf)
		{
			for(int i = 0; i < temp.num-1; i++)
			{
				System.out.print(temp.pair.get(i).key + ",");
			}
			System.out.println(temp.pair.getLast().key);
			
			//1. index값과 key값이 같을 경우 -> 다음 index의 ptr을 따라감.
			//2. 최소 index보다 key가 작음
			//3. 최대 index보다 key가 큼
			//4. key의 값이 두 index의 사이임

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
		//출력까지 구현
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
		 //temp가 leaf node가 됨
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
		//중복 검사
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
				target = target.SplitNode(this, degree); //split해서 만든 subtree의 root를 return.\
				
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
	
	public Node ArrangeNode(Node root, Node origin, int degree) //root는 tree의 root고 node는 split해서 나온 subtree의 root임
	{
//		System.out.println("in ARRANGENODE");
			
		int tempkey = 0;
		Node trial = this;
		for(int i = 0; i < this.height; i++)
		{
			trial = trial.pair.getFirst().ptr;
		}
		tempkey = trial.pair.getFirst().key;
		Node target = FindNonLeafNode(root, tempkey); //상위 subtree에서 key찾기, !!이 node가 root라면?!!
		
		//Node target = target.pair.getFirst().ptr.FindParentNode(root);
		
		//parent를 찾은건가? ㄷㄷ;
		
		//결국 target은 splitnode를 삽입할 node를 찾겠다는거잖슴
			
//			if(target == null)
//			{
//				if(root.height == this.height) //새로 만들어진 subtree의 높이가 root와 같다는건 root node에 추가해야한다는 뜻
//				{
//					target = root;
//				}
//				else //이게 이해가 안됨;; 내가 썻는데 왜 이해를 몬하누 ㅋㅋㅋ; 아 대충 알겟다 그때 뭔생각했는지
//					//맨 오른쪽 leaf node의 parent를 찾고 싶었던건데......
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
//				if(root.height == this.height) //새로 만들어진 subtree의 높이가 root와 같다는건 root node에 추가해야한다는 뜻
//				{
//					target = root;
//				}
//				
//				if(target.height - this.pair.getFirst().ptr.height > 1)
//				{
//					//tempkey가 root의 몇번째인지 알아내서
//					//height 통해서 target찾기
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

			//node 내부에서 key의 대소비교를 해서 맨뒤에 삽입되는거랑 아닌 경우 나눠줘야함. right subtree땜시..
			
			int newkey = this.pair.getFirst().key;
			if(newkey > target.pair.getLast().key)
			{ 
				//맨 끝에 node 붙이기
				boolean check = target.pair.getLast().ptr.leaf;
				
				if(target.pair.getLast().ptr.leaf) //target의 subtree가 root일경우 포인터를 더 이어줘야 함.
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
				//맨 앞에 삽입되는 경우 포인터를 바꿔줘야하더라 ....
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
						target.pair.add(i+1, this.pair.getFirst()); //새로 낑겨들어간 node랑 그 이전 node연결안된상태
						target.pair.add(i+2, new Pair(nextkey, this.ptr));
						
					}
				}
			}
			target.num += 1;
			
			if(target.num == degree)
			{
				target = target.SplitNode(root, degree);
			}
			
			if(target.root) //target이 root라면
			{
				root = target;
				return root;
			}
			
	
		
	
		return target;
	}
	
	
	
	public Node FindLeafNode(int key) //leaf node 찾기
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
			
			if(this.leaf) //처음부터 자식노드였으므로 split후에는 왼쪽 child와 오른쪽 child가 leaf node가 됨
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
			if(this.leaf) //처음부터 자식노드였으므로 split후에는 왼쪽 child와 오른쪽 child가 leaf node가 됨
			{
				LeftChild.leaf = RightChild.leaf = true;
				LeftChild.ptr = RightChild;
				RightChild.ptr = this.ptr;
				RightChild.pair.addFirst(midpair);
				RightChild.num += 1;
				
				//pointer추가
				Node parent = FindNonLeafNode(rootnode, LeftChild.pair.getFirst().key);
				if(parent != null)  //Non leaf중에 key를 포함하는 node가 없음
				{
					Node link = rootnode.FindLeafNode((LeftChild.pair.getFirst().key)-1);
					link.ptr = LeftChild;
				}
				
				/*
				int compare = LeftChild.pair.getFirst().key;
				if(compare >= rootnode.pair.getFirst().key) //root의 첫번째 subtree가 아님
				{
					Node link = rootnode.FindLeafNode(compare-1);
					link.ptr = LeftChild;
				}
				*/
				
			}
			else //중간에 낑긴 노드
			{
				LeftChild.leaf = RightChild.leaf = false;
			}
		
			temp.leaf = false;
			temp.num = 1;
			temp.pair.add(new Pair(midpair.key, LeftChild));
			temp.ptr = RightChild;
			
			
			
			
		}
		//높이 추가부분
		RightChild.height = this.height;
		LeftChild.height = this.height;
		temp.height = this.height +1;
		
		//split 끝
		
		//arrange 합치기
		if(!temp.root)
		{
			temp = temp.ArrangeNode(rootnode, this, degree); //원래 B+ tree와 합치기
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
//			if(this.leaf) //처음부터 자식노드였으므로 split후에는 왼쪽 child와 오른쪽 child가 leaf node가 됨
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
//			if(this.leaf) //처음부터 자식노드였으므로 split후에는 왼쪽 child와 오른쪽 child가 leaf node가 됨
//			{
////				LeftChild.leaf = RightChild.leaf = true;
////				LeftChild.ptr = RightChild;
//				this.leaf = RightChild.leaf = true;
//				this.ptr = RightChild;
////				RightChild.ptr = this.ptr;
//				RightChild.pair.addFirst(midpair);
//				RightChild.num += 1;
//				
//				//pointer추가, target을 재활용하면 왼쪽 leaf랑 포인터를 다시 이어줄 필요가 없음...
////				Node parent = FindNonLeafNode(rootnode, LeftChild.pair.getFirst().key);
////				if(parent != null)  //Non leaf중에 key를 포함하는 node가 없음
////				{
////					Node link = rootnode.FindLeafNode((LeftChild.pair.getFirst().key)-1);
////					link.ptr = LeftChild;
////				}
//				
//			}
//			else //중간에 낑긴 노드
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
//		//높이 추가부분
//		RightChild.height = this.height;
////		LeftChild.height = this.height;
//		temp.height = this.height +1;
//		
//		//split 끝
//		
//		//arrange 합치기
//		if(!temp.root)
//		{
//			temp = temp.ArrangeNode(rootnode, degree); //원래 B+ tree와 합치기
//		}
//		return temp;
	}
	
	
	public Node FindNonLeafNode(Node node, int key) //생각해보니까 Pair말고 Node를 반환해야 재귀함수가 가능할 것 같아서 코드를 다시 짜려고 함.
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
