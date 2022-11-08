import java.awt.Image;
import java.io.* ; 
import java.net.* ; 
import java.util.* ;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class WebClient {

	public static void main(String[] args) throws IOException{
		WebClient client=new WebClient();  
		
		Scanner scanner = new Scanner(System.in);
		
		while(true)
		{
			//Mission 1
			System.out.println("1. GET METHOD");
			System.out.println("2. POST METHOD");
			System.out.print("INSERT NUM > ");
			String tmp = scanner.nextLine();
			int num = Integer.parseInt(tmp);
			System.out.println();

			if(num == 1)
			{
				System.out.print("Insert URL : ");
				String URL = scanner.nextLine();

				String s = client.getWebContentByGet(URL);

				System.out.println(s);
				System.out.println();
			}

			else if(num == 2)
			{
				//Mission 2
				System.out.print("Insert URL : ");
				String URL = scanner.nextLine();

				System.out.print("Insert data : ");
				String data = scanner.nextLine();

				String s = client.getWebContentByPost(URL,data);  
				System.out.println(s);
				System.out.println();
			}

			else
				continue;

//			//Mission 3
//			System.out.print("Insert URL : ");
//			URL = scanner.nextLine();
//
//			System.out.print("Insert data : ");
//			data = scanner.nextLine();
//
//			s = client.getWebContentByPost(URL,data);  
//			System.out.println(s);
//			System.out.println();
//
//			//Mission 4
//			System.out.print("Insert URL : ");
//			URL = scanner.nextLine();
//			String[] split = URL.split(":");
//
//			if(!split[0].equals("127.0.0.1"))
//			{
//				URL url = new URL(URL);
//				Image image = ImageIO.read(url);
//				//s = client.getWebContentByGet(URL);
//
//				JFrame frame = new JFrame();
//				frame.setSize(300, 300);
//				JLabel label = new JLabel(new ImageIcon(image));
//				frame.add(label);
//				frame.setVisible(true);
//			}
		}

		//System.out.println(s);
	}

	public String getWebContentByPost(String urlString,String data) throws IOException {  
		return getWebContentByPost(urlString, data,"UTF-8", 5000);//iso-8859-1  
	}  

	public String getWebContentByGet(String urlString) throws IOException {  
		return getWebContentByGet(urlString, "iso-8859-1", 5000);  
	} 

	public String getWebContentByGet(String urlString, final String charset, int timeout) throws IOException {
		if(urlString == null || urlString.length() == 0)
		{
			return null;
		}
		urlString = (urlString.startsWith("http://") || urlString.startsWith("https://")) ? urlString
				: ("http://" + urlString).intern();
		URL url = new URL(urlString);
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.setRequestMethod("GET");
		

		conn.setRequestProperty("User-Agent", "2018008531/YEONJUSONG/WebClient/ComNet");
		
		if(urlString.endsWith("index.html"))
			conn.setRequestProperty("Accept", "text/html");
		else if(urlString.endsWith("image.jpg"))
			conn.setRequestProperty("Accept", "image/jpeg");
		conn.setConnectTimeout(timeout);
		
		try
		{
			if(conn.getResponseCode() != HttpURLConnection.HTTP_OK)
				return null;
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return null;
		}
		
		InputStream input = conn.getInputStream();
		BufferedReader reader = new BufferedReader(new InputStreamReader(input, charset));
		String line = null;
		StringBuffer sb = new StringBuffer();
		while((line = reader.readLine()) != null)
		{
			sb.append(line).append("\r\n");
		}
		if(reader != null)
			reader.close();
		if(conn != null)
			conn.disconnect();
		
		return sb.toString();
	
	}

	public String getWebContentByPost(String urlString, String data, final String charset, int timeout) throws IOException{
		if(urlString == null || urlString.length() == 0)
			return null;
		
		urlString = (urlString.startsWith("http://") || urlString.startsWith("https://")) ? urlString
				: ("http://" + urlString).intern();
		
		URL url = new URL(urlString);
		HttpURLConnection connection = (HttpURLConnection) url.openConnection();
		
		connection.setDoOutput(true);
		connection.setDoInput(true);
		connection.setRequestMethod("POST");
		
		connection.setUseCaches(false);
		connection.setInstanceFollowRedirects(true);
		
		connection.setRequestProperty("Content-Type", "text/xml;charset=UTF-8");
		
		connection.setRequestProperty("User-Agent", "2018008531/YEONJUSONG/WebClient/ComNet");
		
		connection.setRequestProperty("Accept", "text/xml");
		connection.setConnectTimeout(timeout);
		connection.connect();
		DataOutputStream out = new DataOutputStream(connection.getOutputStream());
		
		byte[] content = data.getBytes("UTF-8");
		
		out.write(content);
		out.flush();
		out.close();
		
		try {
			if(connection.getResponseCode() != HttpURLConnection.HTTP_OK)
				return null;
		} catch (IOException e)
		{
			e.printStackTrace();
			return null;
		}
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream(),charset));
		String line;
		StringBuffer sb = new StringBuffer();
		while((line = reader.readLine()) != null)
			sb.append(line).append("\r\n");
		
		if(reader != null)
			reader.close();
		
		if(connection != null)
			connection.disconnect();
		
		return sb.toString();
	} 
}
