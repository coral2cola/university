import java.sql.Connection;
import java.sql.Date;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.Scanner;

public class Query {
	
	private Connection conn;
	private PreparedStatement pstmt;
	ResultSet rs = null;
	Statement stmt;
	Scanner scan = new Scanner(System.in);
	
	public Query() {
		String dbURL = "jdbc:mysql://localhost:3306/music?serverTimezone=UTC";
		String dbID = "root";
		String dbPW = "1234";
		
		try
		{
			Class.forName("com.mysql.cj.jdbc.Driver");
			conn = DriverManager.getConnection(dbURL, dbID, dbPW);
		} catch(ClassNotFoundException e)
		{
			e.printStackTrace();
		} catch(SQLException e)
		{
			e.printStackTrace();
		}
	}
	
	//query ����
	//insert, update, delete, select ����
	
	/*INSERT*/
	//�������߰�
	public int insertAdmin(String id, String pw)
	{
		String SQL = "INSERT INTO admin VALUES(?,?)";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, id);
			pstmt.setString(2, pw);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ������߰�
	public int insertStreamingUser(String id, String pw, int age, String mgr_id)
	{
		String SQL = "INSERT INTO user VALUES(?,?,?,?)";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, id);
			pstmt.setString(2, pw);
//			pstmt.setString(3, srmName);
//			pstmt.setDate(4, date);
			pstmt.setInt(3, age);
			pstmt.setString(4, mgr_id);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//�����߰�
	public int insertMusic(int num, String name, int tracknum, Date release, String mgr_id, int albumnum, int artistnum)
	{
		String SQL = "INSERT INTO music VALUES(?,?,?,?,?,?,?)";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			pstmt.setString(2, name);
			pstmt.setInt(3, tracknum);
			pstmt.setDate(4, release);
			pstmt.setString(5, mgr_id);
			pstmt.setInt(6, albumnum);
			pstmt.setInt(7, artistnum);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//�ٹ��߰�
	public int insertAlbum(int num, String name, Date release, String mgr_id, int artistnum)
	{
		String SQL = "INSERT INTO album VALUES(?,?,?,?,?)";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			pstmt.setString(2, name);
			pstmt.setDate(3, release);
			pstmt.setString(4, mgr_id);
			pstmt.setInt(5, artistnum);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ƽ��Ʈ�߰�
	public int insertArtist(int num, String name, String debut, String year, String nation, String mgr_id)
	{
		String SQL = "INSERT INTO artist VALUES(?,?,?,?,?,?)";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			pstmt.setString(2, name);
			pstmt.setString(3, debut);
			pstmt.setString(4, year);
//			pstmt.setDate(5, year);
			pstmt.setString(5, nation);
			pstmt.setString(6, mgr_id);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ����� �÷��̸���Ʈ �߰�
	public int insertStreamingPlaylist(int index, String id, int num)
	{
//		String SQL = "INSERT INTO playlist VALUES(?,?,?)";
		String SQL = "SELECT musicNum FROM playlist WHERE userID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, id);
			rs = pstmt.executeQuery();
			
			while(rs.next())
			{
				int music = rs.getInt("musicNum");
				if(music == num)
					return 2;
			}
			SQL = "INSERT INTO playlist VALUES(?,?,?)";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, index);
			pstmt.setString(2, id);
			pstmt.setInt(3, num);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	/*UPDATE*/
	//������ ������Ʈ
	public int updateAdmin(String targetID, String newPW) //targetID == PK
	{
		String SQL = "UPDATE admin SET AdminPW=? WHERE AdminID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newPW);
			pstmt.setString(2, targetID); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ʈ���ֱ����� ������Ʈ (��й�ȣ ����)
	public int updateStreamingUserPW(String targetID, String newPW) //targetID == PK
	{
		String SQL = "UPDATE user SET userPW=? WHERE userID=?";
		int result = 0;
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newPW);
			pstmt.setString(2, targetID); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ʈ���ֱ����� ������Ʈ (���� ����)
	public int updateStreamingUserAge(String targetID, int age) //targetID == PK
	{
		String SQL = "UPDATE user SET userAge=? WHERE userID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, age);
			pstmt.setString(2, targetID); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ʈ���ֱ����� ������Ʈ (manager ID)
	public int updateStreamingUserManager(String targetID, String newID) //targetID == PK
	{
		String SQL = "UPDATE user SET mgr_ID=? WHERE userID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newID);
			pstmt.setString(2, targetID); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//���� ������Ʈ (�̸� ����)
	public int updateMusicName(int targetNum, String newName)
	{
		String SQL = "UPDATE music SET musicName=? WHERE musicNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newName);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//���� ������Ʈ (Ʈ���ѹ� ����)
	public int updateMusicTrack(int targetNum, int newNum)
	{
		String SQL = "UPDATE music SET trackNum=? WHERE musicNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, newNum);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//���� ������Ʈ (�߸ų�¥ ����)
	public int updateMusicReleaseDate(int targetNum, Date newDate)
	{
		String SQL = "UPDATE music SET releaseDate=? WHERE musicNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setDate(1, newDate);
			pstmt.setInt(2, targetNum);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//���� ������Ʈ (�ٹ� ����)
	public int updateMusicAlbum(int targetNum, int newNum)
	{
		//newNum�� ���ο� album��ȣ��
//		String SQL = "UPDATE music SET albumNum=? WHERE musicNum=?";
		String SQL = "SELECT artistNum FROM album where albumNum=?";
		int result = 0;
		int artistNum = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, newNum);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				artistNum = rs.getInt("artistNum");
			}
			
			SQL = "UPDATE music SET albumNum=? WHERE musicNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, newNum);
			pstmt.setInt(2, targetNum); 
			int result1 = pstmt.executeUpdate();
			
			SQL = "UPDATE music SET artistNum=? WHERE musicNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, artistNum);
			pstmt.setInt(2, targetNum); 
			int result2 = pstmt.executeUpdate();
			
			result = result1 & result2;
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//���� ������Ʈ (��Ƽ��Ʈ ����)
	public int updateMusicArtist(int targetNum, int newNum)
	{
		String SQL = "UPDATE music SET artistNum=? WHERE musicNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, newNum);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//���� ������Ʈ (�Ŵ��� ����)
	public int updateMusicManager(int targetNum, String newID)
	{
		String SQL = "UPDATE music SET mgr_ID=? WHERE musicNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newID);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//�ٹ� ������Ʈ (�̸� ����)
	public int updateAlbumName(int targetNum, String newName)
	{
		String SQL = "UPDATE album SET albumName=? WHERE albumNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newName);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//�ٹ� ������Ʈ (�߸��� ����)
	public int updateAlbumReleaseDate(int targetNum, Date newDate)
	{
		String SQL = "UPDATE album SET releaseDate=? WHERE albumNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setDate(1, newDate);
			pstmt.setInt(2, targetNum);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//�ٹ� ������Ʈ (��Ƽ��Ʈ ����)
	public int updateAlbumArtist(int targetNum, int newNum)
	{
		String SQL = "UPDATE album SET artistNum=? WHERE albumNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, newNum);
			pstmt.setInt(2, targetNum); 
			int result1 = pstmt.executeUpdate();
			
			SQL = "UPDATE music SET artistNum=? WHERE albumNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, newNum);
			pstmt.setInt(2, targetNum); 
			int result2 = pstmt.executeUpdate();
			
			result = result1;
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//�ٹ� ������Ʈ (�Ŵ��� ����)
	public int updateAlbumManager(int targetNum, String newID)
	{
		String SQL = "UPDATE album SET mgr_ID=? WHERE albumNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newID);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ƽ��Ʈ ������Ʈ (�̸� ����)
	public int updateArtistName(int targetNum, String newName)
	{
		String SQL = "UPDATE artist SET artistName=? WHERE artistNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newName);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ƽ��Ʈ ������Ʈ (���߳⵵ ����)
	public int updateArtistDebut(int targetNum, String year)
	{
		String SQL = "UPDATE artist SET debut=? WHERE artistNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, year);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ƽ��Ʈ ������Ʈ (Ȱ������ ����)
	public int updateArtistActive(int targetNum, String year)
	{
		String SQL = "UPDATE artist SET year=? WHERE artistNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, year);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ƽ��Ʈ ������Ʈ (���� ����)
	public int updateArtistNation(int targetNum, String nation)
	{
		String SQL = "UPDATE artist SET nation=? WHERE artistNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, nation);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	//��Ƽ��Ʈ ������Ʈ (�Ŵ��� ����)
	public int updateArtistManager(int targetNum, String newID)
	{
		String SQL = "UPDATE artist SET mgr_ID=? WHERE artistNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, newID);
			pstmt.setInt(2, targetNum); 
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	/*DELETE*/
	//�����ڻ���
	public int deleteAdmin(String id)
	{
		String SQL = "DELETE FROM admin where AdminID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, id);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ����ڻ���
	public int deleteStreamingUser(String id)
	{
		//�÷��̸���Ʈ�� ���� ����� ����ڸ� ������ key constraint�� ��ġ�� ����
		int a = deleteStreamingPlaylist2(id);
		if(a == 0)
			return 2;
		String SQL = "DELETE FROM user where userID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, id);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��������
	public int deleteMusic(int num)
	{
		String SQL = "DELETE FROM music where musicNum=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��������
	public int delMusic(String musicName)
	{
		//�÷��̸���Ʈ�� ���� �����ؾ���.
		//���� ���� �� �ٹ��� ��Ƽ��Ʈ�� ���� ������ 0���� �� ��� �ش� �ٹ��� ��Ƽ��Ʈ ����.
		
		int a = deleteStreamingPlaylist3(musicName);
		if(a == 0) //�÷��̸���Ʈ ���� ����
			return 2;
		
		//�����ϰ��� �ϴ� ������ �ٹ��ѹ��� ��Ƽ��Ʈ �ѹ��� ã�ƾ� ��
		//ã�Ƽ� music table�ȿ� ����� ���������
		//1����� �� album �Ǵ� artist�� �����������
		String SQL = "SELECT musicNum,albumNum,artistNum FROM music WHERE musicName=?";
		int result = 0;
		int musicNum = 0;
		int albumnum = 0;
		int artistnum = 0;
		int albumcnt = 0;
		int artistcnt = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, musicName);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				musicNum = rs.getInt("musicNum");
				albumnum = rs.getInt("albumNum");
				artistnum = rs.getInt("artistNum");
			}
			
			SQL = "SELECT albumNum FROM music WHERE albumNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, albumnum);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				albumcnt++;
			}
			
			SQL = "SELECT artistNum FROM music WHERE artistNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, artistnum);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				artistcnt++;
			}
			
//			System.out.println("albumcnt : " + albumcnt);
//			System.out.println("artistcnt : " + artistcnt);
			
			int n1 = deleteMusic(musicNum);
			if(n1 == 0)
				return 0;
			
			int n2 = 0;
			int n3 = 0;
			
//			System.out.println("n1 : " + n1);
			if(albumcnt == 1)
			{
				n2 = deleteAlbum(albumnum);
//				System.out.println("n2 : " + n2);
				if(n2 == 0)
					return 0;
			}
			
			if(artistcnt == 1)
			{
				SQL = "DELETE FROM artist where artistNum=?";
				pstmt = conn.prepareStatement(SQL);
				pstmt.setInt(1, artistnum);
				n3 = pstmt.executeUpdate();
					
//				System.out.println("n3 : " + n3);
				if(n3 == 0)
					return 0;
			}
			
			return 1;
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//�ٹ�����
	public int deleteAlbum(int num)
	{
		//music����
		//album����
		//artist����
		
		String SQL = "SELECT musicNum FROM music WHERE albumNum=?";
		int result = 0;
		int check = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				check = 1;
				break;
			}
			
			if(check == 1)
			{
				SQL = "DELETE FROM music where albumNum=?";
				pstmt = conn.prepareStatement(SQL);
				pstmt.setInt(1, num);
				result = pstmt.executeUpdate();
			}
			
			SQL = "DELETE FROM album where albumNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ƽ��Ʈ����
	public int deleteArtist(int num)
	{
		//music ����
		//album ����
		//artist ����
		
//		String SQL = "DELETE FROM artist where artistNum=?";
		
		String SQL = "SELECT musicNum FROM music WHERE artistNum=?";
		int result = 0;
		int check = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				check = 1;
				break;
			}
			
			int result1;
			if(check == 0)
				result1 = 1;
			else
			{
				SQL = "DELETE FROM music where artistNum=?";
				pstmt = conn.prepareStatement(SQL);
				pstmt.setInt(1, num);
				result1 = pstmt.executeUpdate();
			}
			
			SQL = "DELETE FROM album where artistNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			int result2 = pstmt.executeUpdate();
			
			SQL = "DELETE FROM artist where artistNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, num);
			int result3 = pstmt.executeUpdate();
			
			result = result3;
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	public int deletePlaylist(String id, String name)
	{
		String SQL = "SELECT musicNum FROM music WHERE musicName=?";
//		String SQL = "DELETE FROM playlist where index=?";
		int result = 0;
		int musicNum = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, name);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				musicNum = rs.getInt("musicNum");
			}
			
			SQL = "DELETE FROM playlist WHERE userID=? and musicNum=?";
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, id);
			pstmt.setInt(2, musicNum);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ����� �÷��̸���Ʈ ����
	public int deleteStreamingPlaylist(String id, int index)
	{
		String SQL = "DELETE FROM playlist where playlist.index=? and userID=?";
		int result = 0;
		
		try {
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, index);
			pstmt.setString(2, id);
			result = pstmt.executeUpdate();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ����� �÷��̸���Ʈ ����2
		public int deleteStreamingPlaylist2(String id)
		{
			String SQL = "SELECT * FROM playlist where userID=?";
//			String SQL = "DELETE FROM playlist where userID=?";
			int result = 0;
			
			try {
				pstmt = conn.prepareStatement(SQL);
				pstmt.setString(1, id);
				rs = pstmt.executeQuery();
				if(!rs.next())
					return 1;
				else
				{
					SQL = "DELETE FROM playlist where userID=?";
					pstmt = conn.prepareStatement(SQL);
					pstmt.setString(1, id);
					result = pstmt.executeUpdate();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			return result;
		}
		
		//��Ʈ���ֱ����� �÷��̸���Ʈ ����3
		public int deleteStreamingPlaylist3(String name)
		{
			String SQL = "SELECT musicNum from music where musicName=?";
//			String SQL = "DELETE FROM playlist where userID=?";
			int result = 0;
			int musicNum = 0;
				
			try
			{
				pstmt = conn.prepareStatement(SQL);
				pstmt.setString(1, name);
				rs = pstmt.executeQuery();
				while(rs.next())
				{
					musicNum = rs.getInt("musicNum");
				}
				
//				System.out.println("musicNum : " + musicNum);
				
				if(musicNum == 0) //�ش� �̸��� ������ �������� ����
					return 0;
				
				int check = 0;
				SQL = "SELECT musicNum from playlist";
				pstmt = conn.prepareStatement(SQL);
				rs = pstmt.executeQuery();
				while(rs.next())
				{
					int num = rs.getInt("musicNum");
					if(num == musicNum)
						check++;
				}
//				System.out.println("check : " + check);
				
				if(check == 0)
					return 1;
				if(check != 0)
				{
					SQL = "DELETE FROM playlist where musicNum=?";
					pstmt = conn.prepareStatement(SQL);
					pstmt.setInt(1, musicNum);
					result = pstmt.executeUpdate();
				}
				
			} catch (Exception e)
			{
				e.printStackTrace();
			}
			return result;
		}
	
	/*SELECT*/
	//��ü �����ڼ���
	public int printAdminAll()
	{
		String SQL = "SELECT * FROM admin";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("----------------------------------------");
					System.out.println("ID                  PASSWORD            ");
					System.out.println("----------------------------------------");
					result = 1;
				}
				
				String id = rs.getString("AdminID");
				String pw = rs.getString("AdminPW");
				
				//System.out.print("               "); 15��
//				System.out.println("----------------------------------------");
//				System.out.println("ID                  PASSWORD            ");
//				System.out.println("----------------------------------------");
				System.out.print(id);
				int length = id.length();
				int blank = 20-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				System.out.println(pw);
			}
			System.out.println("----------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//��� ������ ID���
	public int printAdminIDAll()
	{
		String SQL = "SELECT AdminID FROM admin";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("--------------------");
					System.out.println("ID                  ");
					System.out.println("--------------------");
					result = 1;
				}
				
				String id = rs.getString("AdminID");
				System.out.println(id);
			}
			System.out.println("--------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//Ư�� ������ ���� ���
	public int printAdmin(String _id)
	{
		String SQL = "SELECT * FROM admin WHERE AdminID=?";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, _id);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("----------------------------------------");
					System.out.println("ID                  PASSWORD            ");
					System.out.println("----------------------------------------");
					result = 1;
				}
				
				String id = rs.getString("AdminID");
				String pw = rs.getString("AdminPW");

				System.out.print(id);
				int length = id.length();
				int blank = 20-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				System.out.println(pw);
			}
			System.out.println("----------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ����� ��ü���
	public int printStreamingUserAll()
	{
		String SQL = "SELECT * FROM user";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------");
					System.out.println("ID                PASSWORD          AGE               MANAGER ID        ");
					System.out.println("------------------------------------------------------------------------");
					result = 1;
				}
				
				String id = rs.getString("userID");
				String pw = rs.getString("userPW");
				int age = rs.getInt("userAge");
				String mgr_id = rs.getString("mgr_ID");
//				String ticket = rs.getString("srmName");
//				Date date = rs.getDate("srmDate");
				
				System.out.print(id);
				int length = id.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(pw);
				length = pw.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(age);
				String temp = Integer.toString(age);
				length = temp.length();
				blank = 18-length;
				
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				System.out.println(mgr_id);
			}
			System.out.println("------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ʈ���ֱ����� ID�� ���
	public int printStreamingUserIDAll()
	{
		String SQL = "SELECT userID FROM user";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------");
					System.out.println("ID                ");
					System.out.println("------------------");
					result = 1;
				}
				
				String id = rs.getString("userID");
				System.out.println(id);
			}
			System.out.println("------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	//Ư�� ��Ʈ���� ������ ���
	public int printStreamingUser(String _id)
	{
		String SQL = "SELECT * FROM user WHERE userID=?";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, _id);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------");
					System.out.println("ID                PASSWORD          AGE               MANAGER ID        ");
					System.out.println("------------------------------------------------------------------------");
					result = 1;
				}
				
				String id = rs.getString("userID");
				String pw = rs.getString("userPW");
				int age = rs.getInt("userAge");
				String mgr_id = rs.getString("mgr_ID");
//				String ticket = rs.getString("srmName");
//				Date date = rs.getDate("srmDate");
				
				System.out.print(id);
				int length = id.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(pw);
				length = pw.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(age);
				String temp = Integer.toString(age);
				length = temp.length();
				blank = 18-length;
				
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				System.out.println(mgr_id);
			}
			System.out.println("------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//���� ��ü ���
	public int printMusicAll()
	{
		String SQL = "SELECT * FROM music";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------------------------------------------------------------");
					System.out.println("MUSIC NUM         MUSIC NAME        TRACK NUM         RELEASE DATE      MANAGER ID        ALBUM NUM         ARTIST NUM        ");
					System.out.println("------------------------------------------------------------------------------------------------------------------------------");
					result = 1;
				}
				
				int num = rs.getInt("musicNum");
				String name = rs.getString("musicName");
				int track = rs.getInt("trackNum");
				Date date = rs.getDate("releaseDate");
				String mgr_id = rs.getString("mgr_ID");
				int album = rs.getInt("albumNum");
				int artist = rs.getInt("artistNum");
				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(name);
				length = name.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(track);
				temp = Integer.toString(track);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				String tmp = new SimpleDateFormat("yyyy-MM-dd").format(date);
				System.out.print(tmp);
				length = tmp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(mgr_id);
				length = mgr_id.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(album);
				temp = Integer.toString(album);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(artist);
	
			}
			System.out.println("------------------------------------------------------------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//���� �̸� ���
	public int printMusicNameAll()
	{
		String SQL = "SELECT musicName FROM music";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------");
					System.out.println("MUSIC NAME        ");
					System.out.println("------------------");
					result = 1;
				}
				
				String name = rs.getString("musicName");
				System.out.println(name);
			}
			System.out.println("------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//Ư�� ���� ���
	public int printMusic(int _num)
	{
		String SQL = "SELECT * FROM music WHERE musicNum=?";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1, _num);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------------------------------------------------------------");
					System.out.println("MUSIC NUM         MUSIC NAME        TRACK NUM         RELEASE DATE      MANAGER ID        ALBUM NUM         ARTIST NUM        ");
					System.out.println("------------------------------------------------------------------------------------------------------------------------------");
					result = 1;
				}
				
				int num = rs.getInt("musicNum");
				String name = rs.getString("musicName");
				int track = rs.getInt("trackNum");
				Date date = rs.getDate("releaseDate");
				String mgr_id = rs.getString("mgr_ID");
				int album = rs.getInt("albumNum");
				int artist = rs.getInt("artistNum");
				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(name);
				length = name.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(track);
				temp = Integer.toString(track);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				String tmp = new SimpleDateFormat("yyyy-MM-dd").format(date);
				System.out.print(tmp);
				length = tmp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(mgr_id);
				length = mgr_id.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(album);
				temp = Integer.toString(album);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(artist);
	
			}
			System.out.println("------------------------------------------------------------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//�ٹ� ��ü ���
	public int printAlbumAll()
	{
		String SQL = "SELECT * FROM album";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------------------------");
					System.out.println("ALBUM NUM         ALBUM NAME        RELEASE DATE      MANAGER ID        ARTIST NUM        ");
					System.out.println("------------------------------------------------------------------------------------------");
					result = 1;
				}
				
				int num = rs.getInt("albumNum");
				String name = rs.getString("albumName");
				Date date = rs.getDate("releaseDate");
				String mgr_id = rs.getString("mgr_ID");
				int artist = rs.getInt("artistNum");
				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(name);
				length = name.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				String tmp = new SimpleDateFormat("yyyy-MM-dd").format(date);
				System.out.print(tmp);
				length = tmp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(mgr_id);
				length = mgr_id.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(artist);
	
			}
			System.out.println("------------------------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//�ٹ� �̸� ��ü ���
	public int printAlbumNameAll()
	{
		String SQL = "SELECT albumName FROM album";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------");
					System.out.println("ALBUM NAME        ");
					System.out.println("------------------");
					result = 1;
				}
				
				String name = rs.getString("albumName");
				System.out.println(name);
			}
			System.out.println("------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//Ư�� �ٹ� ���
	public int printAlbum(int _num)
	{
		String SQL = "SELECT * FROM album WHERE albumNum=?";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1,_num);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------------------------");
					System.out.println("ALBUM NUM         ALBUM NAME        RELEASE DATE      MANAGER ID        ARTIST NUM        ");
					System.out.println("------------------------------------------------------------------------------------------");
					result = 1;
				}
				
				int num = rs.getInt("albumNum");
				String name = rs.getString("albumName");
				Date date = rs.getDate("releaseDate");
				String mgr_id = rs.getString("mgr_ID");
				int artist = rs.getInt("artistNum");
				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(name);
				length = name.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				String tmp = new SimpleDateFormat("yyyy-MM-dd").format(date);
				System.out.print(tmp);
				length = tmp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(mgr_id);
				length = mgr_id.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(artist);
	
			}
			System.out.println("------------------------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ƽ��Ʈ ��ü ���
	public int printArtistAll()
	{
		String SQL = "SELECT * FROM artist";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------------------------------------------");
					System.out.println("ARTIST NUM        ARTIST NAME       DEBUT YEAR        ACTIVE YEAR       NATION            MANAGER ID        ");
					System.out.println("------------------------------------------------------------------------------------------------------------");
					result = 1;
				}
				
				int num = rs.getInt("artistNum");
				String name = rs.getString("artistName");
				String debut = rs.getString("debut");
				String year = rs.getString("year");
				String nation = rs.getString("nation");
				String mgr_id = rs.getString("mgr_ID");

				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(name);
				length = name.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(debut);
				length = debut.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(year);
				length = year.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(nation);
				length = nation.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(mgr_id);
			}
			System.out.println("------------------------------------------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ƽ��Ʈ ��ü �̸� ���
	public int printArtistNameAll()
	{
		String SQL = "SELECT artistName FROM artist";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------");
					System.out.println("ARTIST NAME       ");
					System.out.println("------------------");
					result = 1;
				}
				String name = rs.getString("artistName");
				System.out.println(name);
			}
			System.out.println("------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//Ư�� ��Ƽ��Ʈ ��ü ���
	public int printArtist(int _num)
	{
		String SQL = "SELECT * FROM artist WHERE artistNum=?";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setInt(1,_num);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------------------------------------------");
					System.out.println("ARTIST NUM        ARTIST NAME       DEBUT YEAR        ACTIVE YEAR       NATION            MANAGER ID        ");
					System.out.println("------------------------------------------------------------------------------------------------------------");
					result = 1;
				}
				
				int num = rs.getInt("artistNum");
				String name = rs.getString("artistName");
				String debut = rs.getString("debut");
				String year = rs.getString("year");
				String nation = rs.getString("nation");
				String mgr_id = rs.getString("mgr_ID");

				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(name);
				length = name.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(debut);
				length = debut.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(year);
				length = year.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(nation);
				length = nation.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(mgr_id);
			}
			System.out.println("------------------------------------------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	// Ư�� ��Ʈ���� ������� ��ü �÷��̸���Ʈ ����
	public int printUserPlaylistAll(String _id)
	{
		String SQL = "SELECT playlist.index,playlist.musicNum,musicName FROM playlist,music WHERE userID=? and playlist.musicNum=music.musicNum";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1,_id);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------");
					System.out.println("INDEX             MUSIC NUM         MUSIC NAME        ");
					System.out.println("------------------------------------------------------");
					result = 1;
				}
				
				int index = rs.getInt("index");
				int num = rs.getInt("musicNum");
				String name = rs.getString("musicName");
				
				System.out.print(index);
				String temp = Integer.toString(index);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(num);
				temp = Integer.toString(num);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(name);
			}
			System.out.println("------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//Ư�� ������� �÷��̸���Ʈ �� �� ����
	public int printUserPlaylist(String _id, int _index)
	{
		String SQL = "SELECT playlist.index,playlist.musicNum,musicName FROM playlist,music WHERE userID=? and playlist.index=? and playlist.musicNum=music.musicNum";
		int result = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1,_id);
			pstmt.setInt(2,_index);
			rs = pstmt.executeQuery();
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------");
					System.out.println("INDEX             MUSIC NUM         MUSIC NAME        ");
					System.out.println("------------------------------------------------------");
					result = 1;
				}
				
				int index = rs.getInt("index");
				int num = rs.getInt("musicNum");
				String name = rs.getString("musicName");
				
				System.out.print(index);
				String temp = Integer.toString(index);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(num);
				temp = Integer.toString(num);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(name);
			}
			System.out.println("------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
			
	public int Music(String musicName, String albumName, String artistName, int trackNum, Date date, String mgr_id)
	{
		String SQL = "SELECT musicNum,musicName FROM music";
	
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			int musicNum = 0;
			while(rs.next())
			{
				int num = rs.getInt("musicNum");
				String mname = rs.getString("musicName");
				musicNum = num+1;
				
				if(musicName.equalsIgnoreCase(mname))
				{
					return 2;
				}
			}
			
			SQL = "SELECT artistNum,artistName FROM artist";
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			
			int artistNum = 0;
			int tmpnum = 0; //artist ���� ������ �� ���̴� ����
			while(rs.next())
			{
				tmpnum = rs.getInt("artistNum");
				String tmpname  = rs.getString("artistName");
				
				if(tmpname.equalsIgnoreCase(artistName))
				{
					artistName = tmpname;
					artistNum = tmpnum;
				}
			}
			
			if(artistNum == 0) //DB�� �ش� ��Ƽ��Ʈ�� �������� ����
			{
				tmpnum += 1;
				System.out.println("*** Add " + artistName + "(artist) In Database ***");
				System.out.print("Debut Year > ");
				String debut = scan.nextLine();
				System.out.print("Active Year > ");
				String active = scan.nextLine();
				System.out.print("Nation > ");
				String nation = scan.nextLine();
				
				int check = insertArtist(tmpnum, artistName, debut, active, nation, mgr_id);
				if(check != 1)
				{
					System.out.println("Add " + artistName + "(artist) In Database error...");
					System.out.println("Exit...");
					return 0; //�Լ�����
				}
				else
				{
					artistNum = tmpnum;
					System.out.println("Add " + artistName + "(artist) In Database Done!");
				}
			}
			
			SQL = "SELECT albumNum,albumName FROM album";
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			
			int albumNum = 0;
			int temp = 0; //album ���� ������ �� ���̴� ����
			while(rs.next())
			{
				temp = rs.getInt("albumNum");
				String tmpname  = rs.getString("albumName");
				
				if(tmpname.equalsIgnoreCase(albumName))
				{
					albumName = tmpname;
					albumNum = temp;
				}
			}
			
			if(albumNum == 0) //DB�� �ش� �ٹ��� �������� ����
			{
				temp += 1;
				System.out.println("*** Add " + albumName + "(album) In Database ***");

				int check = insertAlbum(temp, albumName, date, mgr_id, artistNum);
				if(check != 1)
				{
					System.out.println("Add " + albumName + "(album) In Database error...");
					System.out.println("Exit...");
					return 0; //�Լ�����
				}
				else
				{
					albumNum = temp;
					System.out.println("Add " + albumName + "(album) In Database Done!");
				}
			}
			
			int a = insertMusic(musicNum, musicName, trackNum, date, mgr_id, albumNum, artistNum);
			return a;
			
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int Album(String albumName, String artistName, Date date, String mgr_id)
	{
		String SQL = "SELECT albumNum,albumName FROM album";
	
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			int albumNum = 0;
			while(rs.next())
			{
				int num = rs.getInt("albumNum");
				String mname = rs.getString("albumName");
				albumNum = num+1;
				
				if(albumName.equalsIgnoreCase(mname))
				{
					return 2;
				}
			}
			
			SQL = "SELECT artistNum,artistName FROM artist";
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			
			int artistNum = 0;
			int tmpnum = 0; //artist ���� ������ �� ���̴� ����
			while(rs.next())
			{
				tmpnum = rs.getInt("artistNum");
				String tmpname  = rs.getString("artistName");
				
				if(tmpname.equalsIgnoreCase(artistName))
				{
					artistName = tmpname;
					artistNum = tmpnum;
				}
			}
			
			if(artistNum == 0) //DB�� �ش� ��Ƽ��Ʈ�� �������� ����
			{
				tmpnum += 1;
				System.out.println("*** Add " + artistName + "(artist) In Database ***");
				System.out.print("Debut Year > ");
				String debut = scan.nextLine();
				System.out.print("Active Year > ");
				String active = scan.nextLine();
				System.out.print("Nation > ");
				String nation = scan.nextLine();
				
				int check = insertArtist(tmpnum, artistName, debut, active, nation, mgr_id);
				if(check != 1)
				{
					System.out.println("Add " + artistName + "(artist) In Database error...");
					System.out.println("Exit...");
					return 0; //�Լ�����
				}
				else
				{
					artistNum = tmpnum;
					System.out.println("Add " + artistName + "(artist) In Database Done!");
				}
			}
			
			int a = insertAlbum(albumNum, albumName, date, mgr_id, artistNum);
			return a;
			
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int Artist(String artistName, String debut, String year, String nation, String mgr_id)
	{
		String SQL = "SELECT artistNum FROM artist";
	
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			int artistNum = 0;
			while(rs.next())
			{
				int num = rs.getInt("artistNum");
				if(artistNum < num)
					artistNum = num;
			}
		
			artistNum++;
		
			int a = insertArtist(artistNum, artistName, debut, year, nation, mgr_id);
			return a;
			
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	public int Playlist(String id, String musicName, String artistName, int index)
	{
		//�� �̸��� ��Ƽ��Ʈ �̸��� ���޹���
		//�� �̸��� ���� �ش� tuple�� ã��
		//�� tuple�� artistNum�� artistName�� ���޹��� �̸��� �������� Ȯ��
		//������ -> playlist�� �߰�
		//�ٸ��� -> �����޽������
		
		String SQL = "SELECT musicNum,musicName,artistNum FROM music";
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			rs = pstmt.executeQuery();
			int artistNum = 0;
			int musicNum = 0;
			while(rs.next())
			{
				int tmpnum = rs.getInt("musicNum");
				String tmpname = rs.getString("musicName");
				int tmpartnum = rs.getInt("artistNum");
				
				if(musicName.equalsIgnoreCase(tmpname)) //tuple�� �� �Է¹��� musicName�� ���� �̸��� ����
				{
					//artistName�� ��
					SQL = "SELECT artistNum,artistName FROM artist";
					pstmt = conn.prepareStatement(SQL);
					ResultSet rs1 = pstmt.executeQuery();
					
					while(rs1.next())
					{
//						System.out.println("tmpartnum : " + tmpartnum);
						int arnum = rs1.getInt("artistNum");
						String arname = rs1.getString("artistName");
//						System.out.println("arname : " + arname);
						if(arnum == tmpartnum) //artist tuple�� ���� ��Ƽ��Ʈ��ȣ�� music tuple�� ���� ��Ƽ��Ʈ ��ȣ ����
						{
							if(arname.equalsIgnoreCase(artistName))
							{
								artistNum = arnum;
								musicNum = tmpnum;
							}
						}
					}
					
					if(artistNum == 0 || musicNum == 0) //artist�� �������� ����
					{
						return 2;
					}
					else //playlist�� ���� �߰�
					{
						int a = insertStreamingPlaylist(index, id, musicNum);
						return a;
					}
				}
			}	
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}
	
	//�����˻�
	public int searchMusic(String musicName)
	{
		String SQL = "SELECT musicNum,albumName,artistName FROM music,album,artist WHERE musicName=? and music.albumNum=album.albumNum and music.artistNum=artist.artistNum";
		int result = 0;
		int check = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1,musicName);
			rs = pstmt.executeQuery();
			
//			while(!rs.next())
//				return 2;
			
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------");
					System.out.println("MUSIC NUM         ALBUM NAME        ARTIST NAME       ");
					System.out.println("------------------------------------------------------");
					result = 1;
					check = 1;
				}
				
				int num = rs.getInt("musicNum");
				String album = rs.getString("albumName");
				String artist = rs.getString("artistName");
				
				System.out.print(num);
				String temp = Integer.toString(num);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(album);
				length = album.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(artist);
			}
			if(check == 0)
				return 2;
			System.out.println("------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//�����˻�
	public int searchAlbum(String albumName)
	{
		String SQL = "SELECT musicNum,album.albumNum,musicName,artistName FROM music,album,artist WHERE albumName=? and music.albumNum=album.albumNum and album.artistNum=artist.artistNum";
		int result = 0;
		int check = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1,albumName);
			rs = pstmt.executeQuery();
			
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------------------------");
					System.out.println("ALBUM NUM         MUSIC NUM         MUSIC NAME        ARTIST NAME       ");
					System.out.println("------------------------------------------------------------------------");
					result = 1;
					check = 1;
				}
				
				int num = rs.getInt("musicNum");
				int alnum = rs.getInt("albumNum");
				String music = rs.getString("musicName");
				String artist = rs.getString("artistName");
				
				System.out.print(alnum);
				String temp = Integer.toString(alnum);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(num);
				temp = Integer.toString(num);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(music);
				length = music.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(artist);
			}
			if(check == 0)
				return 2;
			System.out.println("------------------------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
	
	//��Ƽ��Ʈ�˻�
	public int searchArtist(String artistName)
	{
		String SQL = "SELECT musicNum,artist.artistNum,musicName FROM music,artist WHERE artistName=? and artist.artistNum=music.artistNum";
		int result = 0;
		int check = 0;
		
		try
		{
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1,artistName);
			rs = pstmt.executeQuery();
			
			while(rs.next())
			{
				if(result == 0)
				{
					System.out.println("------------------------------------------------------");
					System.out.println("ARTIST NUM        MUSIC NUM         MUSIC NAME        ");
					System.out.println("------------------------------------------------------");
					result = 1;
					check = 1;
				}
				
				int num = rs.getInt("musicNum");
				int alnum = rs.getInt("artistNum");
				String music = rs.getString("musicName");
				
				System.out.print(alnum);
				String temp = Integer.toString(alnum);
				int length = temp.length();
				int blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.print(num);
				temp = Integer.toString(num);
				length = temp.length();
				blank = 18-length;
				for(int i = 0; i < blank; i++)
					System.out.print(" ");
				
				System.out.println(music);
				
			}
			if(check == 0)
				return 2;
			System.out.println("------------------------------------------------------");
			
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		return result;
	}
}
