import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.Scanner;


//main 함수가 있는 class. 
//command-line interface와 관련한 부분

public class BpTree {
	
	public static void main(String[] args)
	{
		Node bptree = new Node();
		
		String command = args[0]; //-c, -i 등의 형태
		String fileName = args[1]; //index.dat
		String info1 = args[2]; //첫번째 숫자 혹은 파일명
		int degree = 0;
		
//		while(true)
//		{
//		Scanner scanner = new Scanner(System.in);
//		
//		String command = scanner.next();
//		String fileName =  scanner.next();
//		String info1 = scanner.next();
//		int degree = 0;
//		
	
		if(command.equals("-c"))
		{
			degree = Integer.parseInt(info1);
			bptree.CreateBptree(fileName, degree);
		}
		else if (command.equals("-i"))
		{
			bptree.InsertData(fileName, info1, degree);
		}
		else if (command.equals("-d"))
		{
			bptree.DeleteData(fileName, info1, degree);
		}
		else if (command.equals("-s"))
		{
			int sNum = Integer.parseInt(info1);
			bptree.SingleSearch(fileName, sNum);
		}
		else if (command.equals("-r"))
		{
			String info2 = args[3]; // 2번째 숫자
			int rNum1 = Integer.parseInt(info1);
//			String info2 = scanner.next();
			int rNum2 = Integer.parseInt(info2);
			bptree.RangeSearch(fileName, rNum1, rNum2);
		}
		else
		{
			System.out.println("error");
		}
		
	}
//	}

}
