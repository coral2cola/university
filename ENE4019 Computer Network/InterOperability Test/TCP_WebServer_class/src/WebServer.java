import java.io.* ;
import java.net.* ;
import java.util.* ;

public final class WebServer {
    public static void main(String argv[]) throws Exception {
    	
    	try {

    		ServerSocket serversocket = new ServerSocket(8080);	
    		
    		// Process HTTP service requests in an infinite loop.
    		while (true) {
    		    // Listen for a TCP connection request.
 			
    			Socket socket = serversocket.accept();
    		    
    		    // Construct an object to process the HTTP request message.
    		    HttpRequest request = new HttpRequest(socket);
    		    
    		    // Create a new thread to process the request.
    		    Thread thread = new Thread(request);    		    
    		    // Start the thread.
    		    thread.start();
    		}
    		
    		
    	}catch(IOException e) {
    		System.out.print(e.getMessage());
    	}catch(Exception e) {
    		System.out.print(e.getMessage());
    	}    	
		
    }
}