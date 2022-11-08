import java.sql.Date;
import java.util.Scanner;



public class DBMS {

	public static void main(String[] args) {

		while(true)
		{
			Query db = new Query();
			Scanner scanner = new Scanner(System.in);
			
			int menu = 4;
			while(menu == 4)
			{
				System.out.println("0. Exit");
				System.out.println("1. Admin Menu");
				System.out.println("2. User Menu");
				System.out.print("Input > ");
				String temp = scanner.nextLine();
				menu = Integer.parseInt(temp);
				System.out.println();
//				menu = scanner.nextInt();
				if(menu == 0 || menu == 1 || menu == 2)
					break;
				System.out.println("warning : Wrong Input!!");
				menu = 4;
			}

			if(menu == 0) // Exit
			{
				System.out.println("Exit from Music DB...");
				break;
			}
			else if(menu == 1) //관리자메뉴
			{
				int choice = -1;
				while(choice == -1)
				{
					System.out.println("0. Return to Previous Menu");
					System.out.println("1. User");
					System.out.println("2. Music");
					System.out.println("3. Album");
					System.out.println("4. Artist");
					System.out.println("5. Admin");
					System.out.print("Input > ");
					String temp = scanner.nextLine();
					choice = Integer.parseInt(temp);
					System.out.println();
					
					if(choice >= 0 && choice <= 5)
						break;
					System.out.println("warning : Wrong Input!!");
					choice = -1;		
				}

				int sel = -1;
				if(choice == 0) //Return to Previous Menu
				{
					System.out.println("Return to Previous Menu...");
					continue;
				}
				else if(choice == 1) //Streaming User
				{
					while(sel == -1)
					{
						System.out.println("0. Return to Previous Menu");
						System.out.println("1. Insert New User");
						System.out.println("2. Delete User");
						System.out.println("3. Update User");
						System.out.println("4. View User List");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						sel = Integer.parseInt(temp);
//						sel = scanner.nextInt();
						System.out.println();
						if(sel >= 0  && sel <= 4)
							break;
						System.out.println("warning : Wrong Input!!");
						sel = -1;
					}
					if(sel == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					else if(sel == 1) //Insert Streaming New User
					{
						System.out.println("*** New User ***");
						System.out.print("ID > ");
						String id = scanner.nextLine();
						
						System.out.print("PW > ");
						String pw = scanner.nextLine();
						
						System.out.print("Age > ");
						String temp = scanner.nextLine();
						int age = Integer.parseInt(temp);
//						int age = scanner.nextInt();
						
						System.out.print("Manager ID > ");
						String mgr_id =  scanner.nextLine();
						
						int a = db.insertStreamingUser(id, pw, age, mgr_id);
						if(a != 1)
						{
							System.out.println("Insert New User error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Insert New User done!");
					}
					else if(sel == 2) //Delete Streaming User
					{
						System.out.println("*** Delete User ***");
						System.out.print("User ID > ");
						String id = scanner.nextLine();
						int a = db.deleteStreamingUser(id);
						if(a == 2)
							System.out.println("Delete User's Playlists error...");
						else if(a != 1)
						{
							System.out.println("Delete User error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete User done!");
					}
					else if(sel == 3) //Update Streaming User
					{
						System.out.println("0. Change Password");
						System.out.println("1. Change Age");
//						System.out.println("2. Change Streaming Ticket Name");
//						System.out.println("3. Change Streaming Start Date");
						System.out.println("2. Change Manager");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int crystal = Integer.parseInt(temp);
						System.out.println();
						if(crystal == 0) //Change User Password
						{
							System.out.println("*** Change Password ***");
							System.out.print("Target ID > ");
							String id = scanner.nextLine();
							System.out.print("New Password > ");
							String pw = scanner.nextLine();
							int a = db.updateStreamingUserPW(id, pw);
							if(a != 1)
							{
								System.out.println("Change Password error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Password done!");
						}
						else if(crystal == 1) //Change User Age
						{
							System.out.println("*** Change Age ***");
							System.out.print("Target ID > ");
							String id = scanner.nextLine();
							System.out.print("New Age > ");
							String str = scanner.nextLine();
							int age = Integer.parseInt(str);
							int a = db.updateStreamingUserAge(id, age);
							if(a != 1)
							{
								System.out.println("Change Age error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Age done!");
						}
						else if(crystal == 2) //Change Streaming User Manager
						{
							System.out.println("*** Change Manager ***");
							System.out.print("Target ID > ");
							String id = scanner.nextLine();
							System.out.print("New Manager ID > ");
							String mgr_id = scanner.nextLine();
							int a = db.updateStreamingUserManager(id, mgr_id);
							if(a != 1)
							{
								System.out.println("Change Manager error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Manager done!");
						}
					}
					else if(sel == 4) //View Streaming User List
					{
						System.out.println("0. View ALL");
						System.out.println("1. View ID ALL");
						System.out.println("2. View One Admin ALL");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int choose = Integer.parseInt(temp);
						System.out.println();
						if(choose == 0) //View ALL
						{
							int a = db.printStreamingUserAll();
							if(a != 1)
							{
								System.out.println("View ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ALL done!");
						}
						else if(choose == 1) //View ID ALL
						{
							int a = db.printStreamingUserIDAll();
							if(a != 1)
							{
								System.out.println("View ID ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ID ALL done!");
						}
						else if(choose == 2) //View One User ALL
						{
							System.out.print("Target ID > ");
							String id = scanner.nextLine();
							int a = db.printStreamingUser(id);
							if(a != 1)
							{
								System.out.println("View ID ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ID ALL done!");
						}
						
					}
				}
				else if(choice == 2) //Music
				{
					while(sel == -1)
					{
						System.out.println("0. Return to Previous Menu");
						System.out.println("1. Insert New Music");
						System.out.println("2. Delete Music");
						System.out.println("3. Update Music");
						System.out.println("4. View Music List");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						sel = Integer.parseInt(temp);
//						sel = scanner.nextInt();
						System.out.println();
						if(sel >= 0  && sel <= 4)
							break;
						System.out.println("warning : Wrong Input!!");
						sel = -1;
					}
					
					if(sel == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					if(sel == 1) //Insert New Music
					{
						System.out.println("*** New Music ***");
						System.out.print("Music Name > ");
						String name = scanner.nextLine();
						System.out.print("Album Name > ");
						String alname = scanner.nextLine();
						System.out.print("Artist Name > ");
						String arname = scanner.nextLine();
						System.out.print("Track Number > ");
						String temp = scanner.nextLine();
						int tracknum = Integer.parseInt(temp);
						System.out.print("Release Date  (yyyy-mm-dd)> ");
						String date = scanner.nextLine();
						Date release = Date.valueOf(date);
						System.out.print("Manager ID > ");
						String mgr_id =  scanner.nextLine();
		
						//조회수와 좋아요 수는 처음에 0으로 초기화 되어야 함.
						int a = db.Music(name, alname, arname, tracknum, release, mgr_id);
//						int a = db.insertMusic(num, name, tracknum, release, mgr_id, albumnum, artistnum);
						if(a == 2)
						{
							System.out.println(name + " Is Already In Database !!");
						}
						else if(a != 1)
						{
							System.out.println("Insert New Music error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Insert New Music done!");
					}
					else if(sel == 2) //Delete Music
					{
						System.out.println("*** Delete Music ***");
						System.out.print("Music Name > ");
						String temp = scanner.nextLine();
						int a = db.delMusic(temp);
						if(a == 2)
							System.out.println("Delete Playlist error...");
						else if(a != 1)
						{
							System.out.println("Delete Music error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete Music done!");
					}
					else if(sel == 3) //Update Music
					{
						System.out.println("0. Change Name");
						System.out.println("1. Change Track Number");
						System.out.println("2. Change Release Date");
						System.out.println("3. Change Album");
//						System.out.println("4. Change Artist");
						System.out.println("4. Change Manager");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int crystal = Integer.parseInt(temp);
						System.out.println();
						if(crystal == 0) //Change Name
						{
							System.out.println("*** Change Name ***");
							System.out.print("Target Music Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Name > ");
							String name = scanner.nextLine();
							
							int a = db.updateMusicName(num, name);
							if(a != 1)
							{
								System.out.println("Change Name error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Name done!");
						}
						else if(crystal == 1) //Change Track Number
						{
							System.out.println("*** Change Track Number ***");
							System.out.print("Target Music Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Track Number > ");
							String name = scanner.nextLine();
							int track = Integer.parseInt(name);
							
							int a = db.updateMusicTrack(num, track);
							if(a != 1)
							{
								System.out.println("Change Track Number error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Track Number done!");
						}
						else if(crystal == 2) //Change Release Date
						{
							System.out.println("*** Change Release Date ***");
							System.out.print("Target Music Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Release Date  (yyyy-mm-dd)> ");
							String date = scanner.nextLine();
							Date newDate = Date.valueOf(date);
							int a = db.updateMusicReleaseDate(num, newDate);
							if(a != 1)
							{
								System.out.println("Change Release Date error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Release Date done!");
						}
						else if(crystal == 3) //Change Album
						{
							System.out.println("*** Change Album ***");
							System.out.print("Target Music Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Album Number > ");
							String name = scanner.nextLine();
							int album = Integer.parseInt(name);
							
							int a = db.updateMusicAlbum(num, album);
							if(a != 1)
							{
								System.out.println("Change Album error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Album done!");
						}
//						else if(crystal == 4) //Change Artist
//						{
//							System.out.println("*** Change Artist ***");
//							System.out.print("Target Music Number > ");
//							String str = scanner.nextLine();
//							int num = Integer.parseInt(str);
//							System.out.print("New Artist Number > ");
//							String name = scanner.nextLine();
//							int artist = Integer.parseInt(name);
//							
//							int a = db.updateMusicArtist(num, artist);
//							if(a != 1)
//							{
//								System.out.println("Change Artist error...");
//								System.out.println("Exit...");
//								//break;
//							}
//							else
//								System.out.println("Change Artist done!");
//						}
						else if(crystal == 4) //Change Manager
						{
							System.out.println("*** Change Manager ***");
							System.out.print("Target Music Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Manager ID > ");
							String mgr_id = scanner.nextLine();
							int a = db.updateMusicManager(num, mgr_id);
							if(a != 1)
							{
								System.out.println("Change Manager error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Manager done!");
						}
					}
					else if(sel == 4) //View Music List
					{
						System.out.println("0. View ALL");
						System.out.println("1. View Name ALL");
						System.out.println("2. View One Music ALL");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int choose = Integer.parseInt(temp);
						System.out.println();
						if(choose == 0) //View ALL
						{
							int a = db.printMusicAll();
							if(a != 1)
							{
								System.out.println("View ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ALL done!");
						}
						else if(choose == 1) //View Name ALL
						{
							int a = db.printMusicNameAll();
							if(a != 1)
							{
								System.out.println("View Name ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View Name ALL done!");
						}
						else if(choose == 2) //View One Music ALL
						{
							System.out.print("Target Music Number > ");
							String tmp = scanner.nextLine();
							int num = Integer.parseInt(tmp);
							int a = db.printMusic(num);
							if(a != 1)
							{
								System.out.println("View One Music ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View One Music ALL done!");
						}
					}
				}
				else if(choice == 3) //Album
				{
					while(sel == -1)
					{
						System.out.println("0. Return to Previous Menu");
						System.out.println("1. Insert New Album");
						System.out.println("2. Delete Album");
						System.out.println("3. Update Album");
						System.out.println("4. View Album List");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						sel = Integer.parseInt(temp);
//						sel = scanner.nextInt();
						System.out.println();
						if(sel >= 0  && sel <= 4)
							break;
						System.out.println("warning : Wrong Input!!");
						sel = -1;
					}
					
					if(sel == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					if(sel == 1) //Insert New Album
					{
						System.out.println("*** New Album ***");
						System.out.print("Album Name > ");
						String alname = scanner.nextLine();
						System.out.print("Artist Name > ");
						String arname = scanner.nextLine();
						System.out.print("Release Date (yyyy-mm-dd) > ");
						String date = scanner.nextLine();
						Date release = Date.valueOf(date);
						System.out.print("Manager ID > ");
						String mgr_id =  scanner.nextLine();
						
						int a = db.Album(alname, arname, release, mgr_id);
						if(a == 2)
						{
							System.out.println(alname + " Is Already In Database !!");
						}
						else if(a != 1)
						{
							System.out.println("Insert New Album error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Insert New Album done!");
						
					}
					else if(sel == 2) //Delete Album
					{
						System.out.println("*** Delete Album ***");
						System.out.print("Album Number > ");
						String temp = scanner.nextLine();
						int num = Integer.parseInt(temp);
						int a = db.deleteAlbum(num);
						if(a != 1)
						{
							System.out.println("Delete Album error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete Album done!");
					}
					else if(sel == 3) //Update Album
					{
						System.out.println("0. Change Name");
						System.out.println("1. Change Release Date");
						System.out.println("2. Change Artist");
						System.out.println("3. Change Manager");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int crystal = Integer.parseInt(temp);
						System.out.println();
						if(crystal == 0) //Change Name
						{
							System.out.println("*** Change Name ***");
							System.out.print("Target Album Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Name > ");
							String name = scanner.nextLine();
							
							int a = db.updateAlbumName(num, name);
							if(a != 1)
							{
								System.out.println("Change Name error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Name done!");
						}
						else if(crystal == 1) //Change Release Date
						{
							System.out.println("*** Change Release Date ***");
							System.out.print("Target Album Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Release Date  (yyyy-mm-dd)> ");
							String date = scanner.nextLine();
							Date newDate = Date.valueOf(date);
							int a = db.updateAlbumReleaseDate(num, newDate);
							if(a != 1)
							{
								System.out.println("Change Release Date error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Release Date done!");
						}
						else if(crystal == 2) //Change Artist
						{
							System.out.println("*** Change Artist ***");
							System.out.print("Target Album Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Artist Number > ");
							String name = scanner.nextLine();
							int artist = Integer.parseInt(name);
							
							int a = db.updateAlbumArtist(num, artist);
							if(a != 1)
							{
								System.out.println("Change Artist error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Artist done!");
						}
						else if(crystal == 3) //Change Manager
						{
							System.out.println("*** Change Manager ***");
							System.out.print("Target Album Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Manager ID > ");
							String mgr_id = scanner.nextLine();
							int a = db.updateAlbumManager(num, mgr_id);
							if(a != 1)
							{
								System.out.println("Change Manager error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Manager done!");
						}
					}
					else if(sel == 4) //View Music Album
					{
						System.out.println("0. View ALL");
						System.out.println("1. View Name ALL");
						System.out.println("2. View One Album ALL");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int choose = Integer.parseInt(temp);
						System.out.println();
						if(choose == 0) //View ALL
						{
							int a = db.printAlbumAll();
							if(a != 1)
							{
								System.out.println("View ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ALL done!");
						}
						else if(choose == 1) //View Name ALL
						{
							int a = db.printAlbumNameAll();
							if(a != 1)
							{
								System.out.println("View Name ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View Name ALL done!");
						}
						else if(choose == 2) //View One Album ALL
						{
							System.out.print("Target Album Number > ");
							String tmp = scanner.nextLine();
							int num = Integer.parseInt(tmp);
							int a = db.printAlbum(num);
							if(a != 1)
							{
								System.out.println("View One Album ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View One Album ALL done!");
						}
					}
				}
				else if(choice == 4) //Artist
				{
					while(sel == -1)
					{
						System.out.println("0. Return to Previous Menu");
						System.out.println("1. Insert New Artist");
						System.out.println("2. Delete Artist");
						System.out.println("3. Update Artist");
						System.out.println("4. View Artist List");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						sel = Integer.parseInt(temp);
//						sel = scanner.nextInt();
						System.out.println();
						if(sel >= 0  && sel <= 4)
							break;
						System.out.println("warning : Wrong Input!!");
						sel = -1;
					}
					
					if(sel == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					else if(sel == 1) //Insert New Artist
					{
						System.out.println("*** New Artist ***");
//						System.out.print("Artist Number > ");
//						String temp = scanner.nextLine();
//						int num = Integer.parseInt(temp);
						
						System.out.print("Artist Name > ");
						String name = scanner.nextLine();
						
						System.out.print("Debut Year (yyyy) > ");
						String year = scanner.nextLine();
//						Date year = Date.valueOf(Year + "-01-01");
						
						System.out.print("Activity Period (yyyy) > ");
						String period = scanner.nextLine();
//						Date period = Date.valueOf(Period + "-01-01");
						
						System.out.print("Nation > ");
						String nation =  scanner.nextLine();
						
						System.out.print("Manager ID > ");
						String mgr_id =  scanner.nextLine();
						
//						int a = db.insertArtist(num, name, year, period, nation, mgr_id);
						int a = db.Artist(name, year, period, nation, mgr_id);
						if(a != 1)
						{
							System.out.println("Insert New Artist error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Insert New Artist done!");
					}
					else if(sel == 2) //Delete Artist
					{
						System.out.println("*** Delete Artist ***");
						System.out.print("Artist Number > ");
						String temp = scanner.nextLine();
						int num = Integer.parseInt(temp);
						int a = db.deleteArtist(num);
						if(a == 0)
						{
							System.out.println("Delete Artist error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete Artist done!");
					}
					else if(sel == 3) //Update Artist
					{
						System.out.println("0. Change Name");
						System.out.println("1. Change Debut Year");
						System.out.println("2. Change Active Year");
						System.out.println("3. Change Nation");
						System.out.println("4. Change Manager");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int crystal = Integer.parseInt(temp);
						System.out.println();
						if(crystal == 0) //Change Name
						{
							System.out.println("*** Change Name ***");
							System.out.print("Target Artist Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Name > ");
							String name = scanner.nextLine();
							
							int a = db.updateArtistName(num, name);
							if(a != 1)
							{
								System.out.println("Change Name error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Name done!");
						}
						else if(crystal == 1) //Change Debut Year
						{
							System.out.println("*** Change Debut Year ***");
							System.out.print("Target Artist Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Debut Year (yyyy) > ");
							String year = scanner.nextLine();
							
							int a = db.updateArtistDebut(num, year);
							if(a != 1)
							{
								System.out.println("Change Debut Year error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Debut Year done!");
						}
						else if(crystal == 2) //Change Active Year
						{
							System.out.println("*** Change Active Year ***");
							System.out.print("Target Artist Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Active Year (yyyy) > ");
							String year = scanner.nextLine();
							
							int a = db.updateArtistActive(num, year);
							if(a != 1)
							{
								System.out.println("Change Active Year error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Active Year done!");
						}
						else if(crystal == 3) //Change Nation
						{
							System.out.println("*** Change Nation ***");
							System.out.print("Target Artist Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Nation > ");
							String nation = scanner.nextLine();
							
							int a = db.updateArtistNation(num, nation);
							if(a != 1)
							{
								System.out.println("Change Nation error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Nation done!");
						}
						else if(crystal == 4) //Change Manager
						{
							System.out.println("*** Change Manager ***");
							System.out.print("Target Artist Number > ");
							String str = scanner.nextLine();
							int num = Integer.parseInt(str);
							System.out.print("New Manager ID > ");
							String mgr_id = scanner.nextLine();
							int a = db.updateArtistManager(num, mgr_id);
							if(a != 1)
							{
								System.out.println("Change Manager error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("Change Manager done!");
						}
					}
					else if(sel == 4) //View Music Artist
					{
						System.out.println("0. View ALL");
						System.out.println("1. View Name ALL");
						System.out.println("2. View One Artist ALL");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int choose = Integer.parseInt(temp);
						System.out.println();
						if(choose == 0) //View ALL
						{
							int a = db.printArtistAll();
							if(a != 1)
							{
								System.out.println("View ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ALL done!");
						}
						else if(choose == 1) //View Name ALL
						{
							int a = db.printArtistNameAll();
							if(a != 1)
							{
								System.out.println("View Name ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View Name ALL done!");
						}
						else if(choose == 2) //View One Artist ALL
						{
							System.out.print("Target Artist Number > ");
							String tmp = scanner.nextLine();
							int num = Integer.parseInt(tmp);
							int a = db.printArtist(num);
							if(a != 1)
							{
								System.out.println("View One Artist ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View One Artist ALL done!");
						}
					}
				}
				else if(choice == 5) //Admin
				{
					while(sel == -1)
					{
						System.out.println("0. Return to Previous Menu");
						System.out.println("1. Insert New Admin");
						System.out.println("2. Delete Admin");
						System.out.println("3. Change Admin Password");
						System.out.println("4. View Admin List");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						sel = Integer.parseInt(temp);
//						sel = scanner.nextInt();
						System.out.println();
						if(sel >= 0  && sel <= 4)
							break;
						System.out.println("warning : Wrong Input!!");
						sel = -1;
					}
					
					if(sel == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					else if(sel == 1) //Insert New Admin
					{
						System.out.println("*** New Admin ***");
						System.out.print("ID > ");
						String id = scanner.nextLine();
						System.out.print("PW > ");
						String pw = scanner.nextLine();
						int a = db.insertAdmin(id, pw);
						if(a != 1)
						{
							System.out.println("Insert New Admin error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Insert New Admin done!");
					}
					else if(sel == 2) //Delete Admin
					{
						System.out.println("*** Delete Admin ***");
						System.out.print("Admin ID > ");
						String id = scanner.nextLine();
						int a = db.deleteAdmin(id);
						if(a != 1)
						{
							System.out.println("Delete Admin error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete Admin done!");
					}
					else if(sel == 3) //Change Admin Password
					{
						System.out.println("*** Change Admin Password ***");
						System.out.print("Target Admin ID > ");
						String id = scanner.nextLine();
						System.out.print("New Password > ");
						String pw = scanner.nextLine();
						int a = db.updateAdmin(id, pw);
						if(a != 1)
						{
							System.out.println("Change Admin Password error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Change Admin Password done!");
					}
					else if(sel == 4) //View Admin List
					{
						System.out.println("0. View ALL");
						System.out.println("1. View ID ALL");
						System.out.println("2. View One Admin ALL");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						int choose = Integer.parseInt(temp);
						System.out.println();
						if(choose == 0) //View ALL
						{
							int a = db.printAdminAll();
							if(a != 1)
							{
								System.out.println("View ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ALL done!");
						}
						else if(choose == 1) //View ID ALL
						{
							int a = db.printAdminIDAll();
							if(a != 1)
							{
								System.out.println("View ID ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ID ALL done!");
						}
						else if(choose == 2) //View One Admin ALL
						{
							System.out.print("Target Admin ID > ");
							String id = scanner.nextLine();
							int a = db.printAdmin(id);
							if(a != 1)
							{
								System.out.println("View ID ALL error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View ID ALL done!");
						}
					}
				}
			}
			else if(menu == 2) //사용자메뉴
			{
				int choice = -1;
				while(choice == -1)
				{
					System.out.println("0. Return to Previous Menu");
					System.out.println("1. Playlist");
					System.out.println("2. Search");
					System.out.print("Input > ");
					String temp = scanner.nextLine();
					choice = Integer.parseInt(temp);
//					choice = scanner.nextInt();
					System.out.println();
					if(choice >= 0 && choice <= 2)
						break;
					System.out.println("warning : Wrong Input!!");
					choice = -1;		
				}

				int sel = -1;
				if(choice == 0) //Return to Previous Menu
				{
					System.out.println("Return to Previous Menu...");
					continue;
				}
				else if(choice == 1) //Streaming PlayList
				{
					while(true)
					{
						System.out.println("0. Return to Previous Menu");
						System.out.println("1. Add New Music into PlayList");
						System.out.println("2. Delete Music in PlayList");
						System.out.println("3. Delete PlayList");
						System.out.println("4. View PlayList");
						System.out.print("Input > ");
						String temp = scanner.nextLine();
						sel = Integer.parseInt(temp);
//						sel = scanner.nextInt();
						System.out.println();
						if(sel >= 0  || sel <= 4)
							break;
						System.out.println("warning : Wrong Input!!");
						sel = -1;
					}

					if(sel == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					else if(sel == 1) //Add New Music into PlayList
					{
						System.out.println("*** Add New Music into PlayList ***");
						//index 수정해야함
						System.out.print("Index > ");
						String temp = scanner.nextLine();
						int index = Integer.parseInt(temp);
						System.out.print("User ID > ");
						String id = scanner.nextLine();
						System.out.print("Music Name > ");
						String name = scanner.nextLine();
						System.out.print("Artist Name > ");
						String arname = scanner.nextLine();
						
						int a = db.Playlist(id, name, arname, index);
//						int a = db.insertStreamingPlaylist(index, id, 0);
						if(a == 2)
						{
							System.out.println("Add New Music into PlayList error...");
							System.out.println("The Music Is Already In Playlist...");
							System.out.println("OR The Music Does NOT Exist...");
						}
						else if(a != 1)
						{
							System.out.println("Add New Music into PlayList error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Add New Music into PlayList done!");
					}
					else if(sel == 2) //Delete Music in PlayList
					{
						System.out.println("*** Delete Music in PlayList ***");
						System.out.print("User ID > ");
						String id = scanner.nextLine();
						System.out.print("Music Name > ");
						String name = scanner.nextLine();
						int a = db.deletePlaylist(id, name);
//						int a = db.deleteStreamingPlaylist(index);
						if(a != 1)
						{
							System.out.println("Delete Music in PlayList error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete Music in PlayList done!");
					}
					else if(sel == 3) //Delete PlayList
					{
						System.out.println("*** Delete PlayList ***");
						System.out.print("User ID > ");
						String id = scanner.nextLine();
						System.out.print("Playlist Index > ");
						String temp = scanner.nextLine();
						int index = Integer.parseInt(temp);
						int a = db.deleteStreamingPlaylist(id, index);
						if(a == 0)
						{
							System.out.println("Delete PlayList error...");
							System.out.println("Exit...");
							//break;
						}
						else
							System.out.println("Delete PlayList done!");
					}
					else if(sel == 4) //View PlayList
					{
						System.out.println("0. View User's Playlist");
						System.out.println("1. View User's One Playlist");
						System.out.print("Input > ");
						String aa = scanner.nextLine();
						int bb = Integer.parseInt(aa);
						
						if(bb == 0) //0. View ALL
						{
							System.out.print("Target User ID > ");
							String id = scanner.nextLine();
							int a = db.printUserPlaylistAll(id);
							if(a != 1)
							{
								System.out.println("View User's Playlist error...");
								System.out.println("Exit...");
								//break;
							}
							else
								System.out.println("View User's Playlist done!");
						}
						else if(bb == 1) //1. View One Playlist
						{
							System.out.print("Target User ID > ");
							String id = scanner.nextLine();
							System.out.print("Target Playlist Number > ");
							String dd = scanner.nextLine();
							int index = Integer.parseInt(dd);
							int a = db.printUserPlaylist(id,index);
							if(a != 1)
							{
								System.out.println("View User's Playlist error...");
								System.out.println("Exit...");
							}
							else
								System.out.println("View User's Playlist done!");
						}
					}
				}
				else if(choice == 2) //Search
				{
					System.out.println("0. Return to Previous Menu");
					System.out.println("1. Search Music");
					System.out.println("2. Search Album");
					System.out.println("3. Search Artist");
					System.out.print("Input > ");
					String tt = scanner.nextLine();
					int sr = Integer.parseInt(tt);
					System.out.println();
					if(sr == 0)
					{
						System.out.println("Return to Previous Menu...");
						continue;
					}
					else if(sr == 1)
					{
						System.out.println("*** Search Music ***");
						System.out.print("Music Name > ");
						String name = scanner.nextLine();
						int a = db.searchMusic(name);
						if(a == 2)
							System.out.println("The Music Does Not Exists...");
						else if(a != 1)
						{
							System.out.println("Search Music error...");
							System.out.println("Exit...");
						}
						else
							System.out.println("Search Music done!");
					}
					else if(sr == 2)
					{
						System.out.println("*** Search Album ***");
						System.out.print("Album Name > ");
						String name = scanner.nextLine();
						int a = db.searchAlbum(name);
						if(a == 2)
							System.out.println("The Album Does Not Exists...");
						else if(a != 1)
						{
							System.out.println("Search Album error...");
							System.out.println("Exit...");
						}
						else
							System.out.println("Search Album done!");
					}
					else if(sr == 3)
					{
						System.out.println("*** Search Artist ***");
						System.out.print("Artist Name > ");
						String name = scanner.nextLine();
						int a = db.searchArtist(name);
						if(a == 2)
							System.out.println("The Artist Does Not Exists...");
						else if(a != 1)
						{
							System.out.println("Search Artist error...");
							System.out.println("Exit...");
						}
						else
							System.out.println("Search Artist done!");
					}
				}
			}
		}	
	}
}