#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<dos.h>//Sound nosound delay
#include<stdlib.h> //Exit

/*Global Functions Prototype Declarations*/
void box(int x1,int y1,int x2,int y2, char *text); //makes box
void intro(void); //Startup page;

//Function related to WELCOME
void welcome(void);//Determines Whom to welcome?? Admin or Guest
int guest_welcome(void); //FOR Guest or NOT
int admin_welcome(void); //For ADMIN or not
void home(void); //MAIN MENU OF THE PROGRAMME
void html(int htm);// if htm=0 ALL DATABASE if htm=1 SINGLE BOOK
void printer(int prn); //To print the whole database
void misuse(void);// ENDs program bcoz of Unauthorized access if trial > 3
void bye(void);// The Last THANK YOU SCREEN

void setscreen(void); //Sets the Screen and Key Frames
void status(char *); //To display changing status of screen
//void clear_status(void); //Clears Ststus box
void which_view(void); // Which view is being used??
void tabview_set(void); //Sets tab view area
void view_set(void);//Set one at a time area
void oneatimeview_set(void); // Sets One at a time FIELDS
void oneatime_show(void); //Display Records one at a time in the FIELDS
void tabview_show(void); //Display in Tabular form form
void bulet(int a,int b); // Bullet for tab view selection


/*Special Functions*/
void mainloop(void); //The main loop that controls display screen
void addbook(int);//Adding book through keyboard
int editbook(void);// Edit book info stored
int searchbook(void);
int writehtm(char *);//Creates a HTM FILE Under C:\
int writebook(int editadd); //To write books to file,1 at a time
			    //if editadd=1(ADD) returns 1 on success
			    //if edtadd=2(EDIT) returns 2 on success
int read_data(void); //Reads data from datasource
void flush(FILE *stream);//flush the data to file without closing it
int input(int x,int y, char *,int length);
int password(int x,int y, char *pass,int length);

void cls(void); // For both views clears USER DATA DISPLAY AREA
void clsview(void); //Clears the fields to print next record for 1 on 1 view
void clstab(void); //Clears the 26 items of TABVIEW

//flags
int view=1; // 0 - guest 1-Admin
int tabview=0; // 0- off(1/1 view)(DEFAULT) 1-On
int truefalse=0;//To check if a function did work or not True=1 False=0
int issue=1; //Not issued
int row=13; //Row no for Bulet

/*Integer Values used for Control*/
unsigned char ch='n', ch2='n'; //Ch to hold user input- a character at a time
int x=6,y=8,i,j; // TO set screen col, row & Loop variables
int len;//For length of strings
int unauth=0; //Counts Unauthorized Access number
long int recno=0; //no of records record used as INDEX for book array
long int recold=0; //Temp store for Switching betn diff modes
long int maxrec=0,rec_size;
int count=0; //For counting purpose

struct library
{
char name[30],author[30],pub[30],edition[5],isbn[30];
char lc[5];
char accno[15],bookno[15];
char issue_to[20],issue_date[11],due_date[11];
};

struct library book;
struct library tmp;// To use to edit Book info
FILE *fp,*ft;

char *pass;
char pword[]="bhu1st";
char *field="NAME(S) OF THE BOOK";
char *title="THE LIBRARY, HIMALAYA COLLEGE OF ENGINEERING, KALOPUL";
char *bullet="->";
char *msg;
char *tempo; //Temporary character storage

/*------------Main Function Starts Here----------------*/
void main(void)
{
textcolor(BLACK);
textbackground(BLACK);
clrscr(); //not needed here bcoz---> intro does it

/*To make user feel that it's Initializing*/
textcolor(RED);
gotoxy(32,22);
cprintf("Initializing");
for(i=46;i<50;i++)
cprintf(".");//delay();}
delay(2000);
/*----File Initialization---*/
rec_size=sizeof(book);
fp=fopen("library.sjb","ab+");
       //fp=fopen("c:\\library.sjb","rb+");
	if (fp==NULL)	bye();
	else
	{
	read_data(); // Reads data at the initialization time to find
	maxrec--;    // maxrec, which is the maxm no of data in the file
	}            //Decreased by one to avoid COUNTER Error.
/*--Intro Screen of the Program--*/
intro();
welcome();
home();

for(;;) //HOME LOOP OF THE PROGRAM
{
fflush(stdin); //Flushes the stdin input device
ch=tolower(getch());
if (ch==32||ch==13) //IF ALLOWS TO ENTER MAIN LOOP
	{
	setscreen();
	which_view();
	if(maxrec==-1) // Maxrec is decremented for the case of exiting database
	status("Database empty.");
	else
	status("The LIBRARY, welcomes you.");
	mainloop();
	home();
	}// IF THAT ALLOWED TO ENTER MAIN LOOP IS CLOSED NOW
else if (ch==27) bye(); //End of program
else if(ch=='p') printer(0); //Call to printer() from HOME
else if(ch=='x') html(0); //Call to html() from HOME
}// HOME loop of the programme
}
/*------------Main Function Ends Here----------------*/

/*-------------------------------------------------------------
	       *USER DEFINED FUNCTIONS*
--------------------------------------------------------------*/

/*--This Function is main USER INTERACTING FUNCTION--*/
void mainloop(void)
{
	for(;;)
	{       fflush(stdin);
		ch=tolower(getch());
		if (ch==32) //SPACE DETECTED
		    {
		    if(recno>=0 && recno<maxrec)
		       {
			 if(tabview==0)
				{
				recno++;
				read_data();
				clsview();
				oneatime_show();
				if(recno == maxrec)
				status("Database - Right Boundary.");
				else
				status("<space> Next Record ->");
				}
			 else status("<space> Switch to One a time view.");
		      }
		    }

		else if (ch==13) //Refresh
		    {
		    setscreen();
		    which_view();
		    status("<CR> Data Refreshed.");
		    }

		else if (ch=='t')
		    {
		    if(tabview==0)
		    {
		    recold=recno;
		    tabview=1;
		    }

		    else
		    {
		    tabview=0;
		    }
		    setscreen();
		    which_view();
		    status("<t> Switch between tab/oneatime view.");
		    }
		else if (ch=='a')
		    {  if (view==1 && tabview==0)
		       {
			recold=recno;
			recno=maxrec+1;
			clsview();
			status("<a>  Please Enter the Book Info.");
			addbook(1);
			//setscreen();
			clsview();
			oneatime_show();
			//which_view();
			//status("Press <H> For Immediate Help.");
		       }
		       else if (tabview==1) status("<a> Switch to one-a-time view.");
		    else  status("<a> You ar not ADMIN!!!.");
		    }
		else if (ch=='e') //edit info
		    {
		    if (view==1)
		       {
			status("Please Enter Records To Modify.");
			truefalse=editbook();
			if(truefalse==0) //if strcmp didn't found records then comes here
			status("Sorry, book number didn't match with stored one.");
			}
		       else if (tabview==1) status("<a> Switch to one-a-time view.");
		       else  status("<a> You ar not ADMIN!!!.");

		    }
	       else if (ch=='f') //edit info
		    {
			if (view==1)
			{
			truefalse=searchbook();
			if(truefalse==0) //if strcmp didn't found records then comes here
			status("Sorry, record not match.");
			else  status("<a> You ar not ADMIN!!!.");
			}
		    }
		else if (ch=='d') //delete
		    {
		    /*strcpy(tempo,book.bookno);
		    ft=fopen("temp.sjb","wb");
		    rewind(fp);
		    while(fread(&tmp,rec_size,1,fp)==1)
		    {
		     if(strcmp(tmp.bookno,tempo)!=0)
		      {fwrite(&tmp,rec_size,1,ft);
		      }
		    else  truefalse=0;
		    }
		    fclose(fp);
		    fclose(ft);
		    remove("library.sjb");
		    rename("temp.sjb","library.sjb");
		    fp=fopen("library.sjb","rb+");
		    recno=0;
		    maxrec=0;
		    read_data(); // Reads data at the initialization time to find
		    maxrec--;

		    if(truefalse==0)*/
		    status("<d> Record can't be deleted.");
		    }
		else if(ch=='x') {html(1);}
		else if (ch==27) break;
		else if(ch==0)
				{//i.e ch==0 if extended code,
				ch2 = getch(); // read second code
				if(ch2==59)
				    status("Function key 1");
				else if(ch2==60)
				    status("Function key 2");
				else if(ch2==75)//LEFT ARROW
				    {
					if(recno>0 && recno<=maxrec)
						{
						if(tabview==0)
						{
						recno--;
						read_data();
						clsview();
						oneatime_show();
						if(recno==0)
						status("Database - Left Boundary.");
						else
						status("<l-arrow> <- Previous Record");
						}
						else status("<space> Switch to One a time view.");
						}
				     }
				else if(ch2==77) //Right Arrow
				    {

				    if(recno>=0 && recno<maxrec)
					{
					if(tabview==0)
					{
					recno++;
					read_data();
					clsview();
					oneatime_show();
					if(recno == maxrec)
					status("Database - Right Boundary.");
					else
					status("<r-arrow> Next Record ->");
					}
					else status("<r-arrow> Switch to One a time view.");
					}
				    }
				else if(ch2==80)  //Down arrow <p>
				    {
				    if (tabview==1)
					{
					if(row <13+25 || recno <maxrec)
					{row++;recno++;
					//if (row>=26) row=13;
					bulet(6,row);
					status("<d-arrow> Select Down"); }
					}
				    else if(tabview==0)  status("<d-arrow> Switch to tabular view.");
				    else printer(1);
				    }
				else if(ch2==72)  //up arrow  <h>
				    {
				    if (tabview==1)
					  {if(row>13 && recno >1) {row--;recno--;} else row=13+25;
					  bulet(6,row);
					  status("<u-arrow> Select Up");
					  }
				    else  status("Please do some Hit n Trial First.");
				    }
				else status("Under construction.");
				}//EXTENDED CODE SCANNED
	}//MAIN LOOP OF THE PROGRAMME
}


void intro(void) // INITIAL STARTUP SCREEN
{
clrscr();
textcolor(GREEN);
box(15,10,65,30,"WELCOME");
textcolor(YELLOW+BLINK);
gotoxy(34,12);cprintf("THE LIBRARY");
textcolor(YELLOW);
gotoxy(34,13);cprintf("컴컴컴컴컴�");
textcolor(RED);
gotoxy(16,15);cprintf("               BY - `SAJEB' GROUP");
textcolor(BLUE);
gotoxy(16,17);cprintf("                 *BHUPAL SAPKOTA*");
gotoxy(16,19);cprintf("               *JANAK RAJ PATHAK*");
gotoxy(16,21);cprintf("                *SAMIR THAPA*");
textcolor(RED);
gotoxy(16,23);cprintf("       A PROJECT IN C (BY BATCH - 062 BCT)");
gotoxy(16,25);cprintf("HIMALAYA COLLEGE OF ENGINEERING - 0977-01-4438702");
gotoxy(16,26);cprintf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�");
textcolor(YELLOW);
box(28,27,50,29,"");
gotoxy(30,28);cprintf("Affiliated  To T.U.");
for(;;){if (ch==32||ch==13) break;ch=tolower(getch());}
}

void welcome(void)
{
	for(;;)
	{      if (truefalse==0)
	       { truefalse=guest_welcome();
		 if (truefalse==1)
			{truefalse=admin_welcome();
				if(truefalse==1)
				{view=1;//printf("Admin Welcome");
				break;
				}//admin welcome
			}
		  else
		     {view=0; //printf("Guestwelcome");
		     break;
		     }//i.e Guestwelcome
		}
	else truefalse=admin_welcome();
	if(truefalse==1)
	view=1; //printf("ADMIN WELCOME");}//Admin welcome
}
}

int guest_welcome(void)
{
clrscr();
textcolor(GREEN);
box(15,10,65,30,"THE LIBRARY : GUEST LOGIN");
textcolor(RED);
gotoxy(35,20);cprintf("WELCOME!");
gotoxy(35,21);cprintf("컴컴컴컴");
textcolor(GREEN);
gotoxy(40,22);cprintf("GUEST");
for(;;)
{ch=tolower(getch());
 if (ch==32||ch==13) return 0;
 else if (ch=='a') return 1;
}
}

int admin_welcome(void)
{
clrscr();
textcolor(GREEN);
box(15,10,65,30,"THE LIBRARY : ADMINISTRATOR LOGIN");
textcolor(RED);
gotoxy(35,20);cprintf("WELCOME!");
gotoxy(35,21);cprintf("컴컴컴컴");
textcolor(GREEN);
gotoxy(40,22);cprintf("ADMINISTRATOR");
textcolor(WHITE);
gotoxy(30,26);cprintf("Password : ");
truefalse=password(42,26,pass,8);
if (truefalse==1) // If Password is Entered
	 {if ((strcmp(pass,pword)==0))
		{
		// If entered password matches the stored one
		textcolor(WHITE);
		gotoxy(30,26);cprintf("WELCOME ADMIN!!!            ");
		delay(1000);
		return 1;
		}
	  else 	{
		// If entered password don't matches the stored one
		textcolor(RED);
		gotoxy(30,28);cprintf("UNAUTHORIZED!!!        ");
		delay(1000);
		unauth++;
		if (unauth==3) misuse();
		truefalse=0;
		view=0;
		return 0;
		//guest_welcome();
		}
	}
else  {view=0;return 0;}// If Password is escaped //whom_welcome();
}


void home(void)
{
clrscr();
//textcolor(BLUE);
//for(i=19,j=14,k=61,l=31;i!=17 ;i--,j--,k++,l++)
//box(i,j,k,l,"");
textcolor(RED);
box(18,13,62,32,"");
textcolor(BLUE);
box(19,14,61,31,"");

textcolor(GREEN);
if (view==1)
box(20,15,60,30,"THE LIBRARY : ADMIN HOME");
else
box(20,15,60,30,"THE LIBRARY : GUEST HOME");


textcolor(WHITE);
gotoxy(25,18);
cprintf("View/Edit Database - <spc/enter>");
gotoxy(30,20);
cprintf("Export to <HTML> -<x>");
gotoxy(30,22);
cprintf(" Print Database -<p>");

textcolor(GREEN);
box(32,26,48,28,"");
textcolor(WHITE);
gotoxy(34,27);
cprintf("Exit - <esc>");
}

void html(int htm)
{
char htm_file[8];
if (htm==0) // Called form HOME
	{
	clrscr();
	textcolor(BLUE);
	box(20,15,60,30,"THE LIBRARY : HTML Extractor");
	textcolor(GREEN);
	box(32,26,47,28,"");
	textcolor(WHITE);
	gotoxy(33,27);
	cprintf("Cancel - <esc>");

	textcolor(WHITE);
	gotoxy(22,19);
	cprintf("Enter name of file without extension");
	gotoxy(22,22);
	cprintf("File name:");
	i=input(33,22,htm_file,8);
	clrscr(); // Clears Screen after file input
	textcolor(BLUE);
	box(20,15,60,30,"THE LIBRARY : HTML Extractor");
	if(writehtm(htm_file))
	{
	sprintf(msg,"File %s.htm exported",htm_file);
	gotoxy(29,22);cprintf(msg);}
	else
	{gotoxy(32,22);cprintf("Some error! Retry.");}
	delay(1500);
	//for(;;){if (ch==32||ch==13||ch==27) break;ch=tolower(getch());}
	//call input function
	home();
	}

else    {    //To EXPORT only a Book info

	status("Please enter name of HTML file without extension");
	delay(2000);
	status("");//Clears Status bar
	gotoxy(15,8+36);textcolor(RED);cprintf("File Name : ");
	i=input(26,8+36,htm_file,8);
	if(writehtm(htm_file))
	{
	sprintf(tempo,"File %s.htm exported",htm_file);
	status(tempo);
	}
	else status("Some error! Retry.");
	ch='n';
}//For Loop
}


void printer(int prn)
{
if (prn==0) //To print the whole database
	{
	clrscr(); // Called form HOME
	textcolor(BLUE);
	box(20,15,60,30,"THE LIBRARY : PRINT BOOK DATABASE");
	textcolor(GREEN);
	box(32,26,47,28,"");
	textcolor(WHITE);
	gotoxy(33,27);
	cprintf("Cancel - <esc>");

	textcolor(WHITE);
	gotoxy(25,19);
	cprintf("ARE YOU SURE YOU WANT TO PRINT");
	gotoxy(25,22);
	cprintf("    THE WHOLE DATABASE");
	gotoxy(25,24);
	cprintf("          (Y/N) ?");
	for(;;)
	{
	ch=tolower(getch());
	textcolor(BLUE);
		if(ch=='y')
		{clrscr(); // Clears Screen after file input
		 box(20,15,60,30,"THE LIBRARY : PRINT BOOK DATABASE");
		 gotoxy(32,22);cprintf("Database Printed. ");break;}

	else if(ch=='n')
		{clrscr(); // Clears Screen after file input
		box(20,15,60,30,"THE LIBRARY : PRINT BOOK DATABASE");
		gotoxy(32,22);cprintf("Some error! Retry. ");break;}
	else if (ch==32||ch==27||ch==13) break;
	}
	delay(1000);
	home();
	}
else  //To print a single database
	{
		status("Do you want to print the current book info(Y/N):");
		for(;;)
		{
		ch=tolower(getch());
		if (ch=='y') {status("Current book info printed.");break;}
		else if (ch=='n') {status("Ok!");break;}
		}//For loop
	}
}

void misuse(void) //Ends programme if unauthorized access trial = 3!!!
{
clrscr();
textcolor(GREEN);
box(15,10,65,30,"WE MUST SAY SORRY!!!");
textcolor(YELLOW);
gotoxy(23,17);cprintf("   We feel that you are trying");
gotoxy(23,19);cprintf(" to willingly misuse the programme.");
gotoxy(23,21); cprintf(" So we must end your session now!!!");
textcolor(WHITE);
gotoxy(27,26);cprintf("SORRY FOR INCONVENIENCE, USER");
getch();bye();getch();exit(EXIT_SUCCESS); //exit(0)
}

void bye(void) //The last thank you screen
{
clrscr();
textcolor(RED);
box(8,3,72,47,"");
textcolor(GREEN);
box(9,4,71,46,"");
textcolor(BLUE);
box(10,5,70,45,"THANK YOU");

textcolor(YELLOW);
gotoxy(25,7);
cprintf("OUR SPECIAL THANKS GOES TO");
gotoxy(20,8);
textcolor(GREEN);
cprintf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
gotoxy(25,9);
cprintf("       OUR FAMILY");
gotoxy(25,11);
cprintf("       OUR FRIENDS");
gotoxy(25,13);
cprintf("         OUR SIRS");
gotoxy(11,15);
cprintf("  HIMALAYA COLLEGE OF ENGINEERING (T.U) - 0977-01-4438702");
gotoxy(25,17);
cprintf("          and");
gotoxy(25,19);
cprintf("       OUR USERS");
textcolor(YELLOW);
gotoxy(25,22);
cprintf("   SPECIAL CREDITS TO");
textcolor(GREEN);
gotoxy(25,23);
cprintf("컴컴컴컴컴컴컴컴컴컴컴컴�");

textcolor(RED);
box(27,24,47,26,"");
gotoxy(29,25);
cprintf("THE `SAJEB' GROUP");

textcolor(YELLOW);
gotoxy(25,28);
cprintf("       Samir Thapa");
gotoxy(25,29);
cprintf("<Procedural Fn. Development>");
textcolor(DARKGRAY);gotoxy(25,30);
cprintf("   zzz@gmail.com");

textcolor(YELLOW);
gotoxy(25,32);
cprintf("     Janak Raj Pathak");
gotoxy(25,33);
cprintf("     <File Handling>");
textcolor(DARKGRAY);gotoxy(25,34);
cprintf("   zzz@gmail.com");

textcolor(YELLOW);
gotoxy(25,36);
cprintf("      Bhupal Sapkota");
gotoxy(22,37);
cprintf("<Interface Design & Overall Development>");
textcolor(DARKGRAY);gotoxy(25,38);
cprintf("    zzz@gmail.com");

textcolor(WHITE);
gotoxy(25,43);
cprintf("  <PRESS ANY KEY TO END>");
fclose(fp); //File is closed at last
getch();exit(EXIT_SUCCESS); //exit(0)
}


int password(int x,int y, char *pass,int length)
{
gotoxy(x,y);
i=0;
for(;;)
{
ch=tolower(getch());
if (ch==13)
{
*(pass+i)='\0';
return 1;
}
if (ch==27) return 0;
if(i<length)
{*(pass+i)=ch;i++;cprintf("%c",'*');;}
else {sound(1000);delay(300);nosound();}
}//For loop
}

int input(int x,int y, char *in,int length)
{
textcolor(WHITE);
gotoxy(x,y);
i=0;
for(;;)
{
fflush(stdin);
ch=tolower(getch());
if (ch==13)
	{
	*(in+i)='\0';
	return 1;
	}
if (ch==27) return 0;
if(i<length)
{*(in+i)=ch;i++;cprintf("%c",ch);}
else {sound(1000);delay(300);nosound();}
}//For loop
}

void setscreen(void) // THE DIVISION OF SCREEN TO DIFFERENT SECTIONS
{
textbackground(BLACK);
clrscr();
//Body box
//SETS USER DATA DISPLAY AREA FOR one on one view or tabview
if(view==1)//Admin
  {textcolor(BLUE);box(1,1,80,49,"THE LIBRARY : ADMIN");}
else //GUEST
  {textcolor(BLUE);box(1,1,80,49,"THE LIBRARY : GUEST");}

//Title box
textcolor(BLUE);box(2,2,79,4,"");
textcolor(YELLOW);len=strlen(title);gotoxy(77/2-len/2,3);cprintf(title);

textcolor(DARKGRAY);
box(6-2,8+34,6+49,47,"Status/Help");

//HELP BOX
textcolor(LIGHTGREEN);
box(57,6,79,8,"Record No.");

//General Keys box
box(x+51,y+2,79,y+10,"General Keys");
textcolor(WHITE);
gotoxy(x+52,y+4);
cprintf("next- <spc/r-arrow>");
gotoxy(x+52,y+6);
cprintf("    back- <l-arrow>");
gotoxy(x+52,y+8);
cprintf("        home- <esc>");

//Procedural keys
textcolor(LIGHTGREEN);
box(x+51,y+12,79,y+22,"Precedural Keys");
if (view==1) textcolor(WHITE);
else textcolor(DARKGRAY);
gotoxy(x+52,y+14);
cprintf("      add book- <a>");
gotoxy(x+52,y+16);
cprintf("     edit info- <e>");
gotoxy(x+52,y+18);
cprintf("        delete- <d>");
gotoxy(x+52,y+20);
cprintf(" issue/unissue- <i/u>");

//Sepcial Function Keys
textcolor(LIGHTGREEN);
box(x+51,y+24,79,y+34,"Special Fn Keys");
textcolor(WHITE);
gotoxy(x+52,y+26);
cprintf("     find book- <f>");
if(tabview==0) textcolor(DARKGRAY);
else textcolor(WHITE);
gotoxy(x+52,y+28);
cprintf("     sort book- <s>");
textcolor(WHITE);
gotoxy(x+52,y+30);
cprintf("export to html- <x>");
gotoxy(x+52,y+32);
cprintf("   toggle view- <t>");

//Developed by
textcolor(LIGHTGREEN);
box(x+51,43,79,48,"Developed By");
textcolor(YELLOW);
gotoxy(x+52,44);
cprintf("  (C) SAJEB GROUP");
gotoxy(x+52,45);
cprintf("Phone 0977-01-4438702");
gotoxy(x+52,46);
cprintf("     HCOE,KALOPUL");
gotoxy(x+52,47);
cprintf("      2062 BCT");
}

void which_view(void)
{
  if(tabview==0){cls();view_set();oneatimeview_set();oneatime_show();}
  else {cls();tabview_set();tabview_show();bulet(6,row);}
}

void tabview_set(void) //Sets tab view area
{
textcolor(LIGHTGREEN);
box(x-3,y-2,x+50,48,"Book & Status Information");

textcolor(RED);
box(x-2,y,x+49,y+32,"Books");

textcolor(BLUE);
gotoxy(x,y+2);
cprintf("%s",field);
gotoxy(x,y+3);
cprintf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�");
}

void view_set(void)
{
//Info Frames
textcolor(LIGHTGREEN);
box(x-3,y-2,x+50,48,"Book & Status Information");
textcolor(RED);
box(x-2,y,x+49,y+12,"General Information");
box(x-2,y+14,x+49,y+22,"Library Information");
box(x-2,y+24,x+49,y+32,"Book Issue Information");
}

void oneatimeview_set(void) //Display Records one at a time  FIELDS
{
//Format for Displaying records
/*General Info*/
textcolor(WHITE);
gotoxy(6,8+2);
cprintf("Book Name");
gotoxy(6,8+4);
cprintf("Author");
gotoxy(6,8+6);
cprintf("Publication");
gotoxy(6,8+8);
cprintf("Edition");
gotoxy(6,8+10);
cprintf("ISBN");
/*Library Info*/
textcolor(WHITE);
gotoxy(6,8+16);
cprintf("Library Copies");
gotoxy(6,8+18);
cprintf("Accession No.");
gotoxy(6,8+20);
cprintf("Book No.");
/*Issue Info*/
textcolor(WHITE);
gotoxy(6,8+26);
cprintf("Issue To");
gotoxy(6,8+28);
cprintf("Issue Date");
gotoxy(6,8+30);
cprintf("Due Date");
textcolor(BROWN);
for (i=8+2;i<=8+10;i=i+2)
      {gotoxy(6+15,i);
      cprintf(":");}

for (i=8+16;i<=8+20;i=i+2)
      {gotoxy(6+15,i);
      cprintf(":");}

for (i=8+26;i<=8+30;i=i+2)
      {gotoxy(6+15,i);
      cprintf(":");}
}

void oneatime_show(void)
{
textcolor(WHITE);
gotoxy(6+18,8+2);
cprintf("%s",book.name);
gotoxy(6+18,8+4);
cprintf("%s",book.author);
gotoxy(6+18,8+6);
cprintf("%s",book.pub);
gotoxy(6+18,8+8);
cprintf("%s",book.edition);
gotoxy(6+18,8+10);
cprintf("%s",book.isbn);

gotoxy(6+18,8+16);
cprintf("%s",book.lc);
gotoxy(6+18,8+18);
cprintf("%s",book.accno);
gotoxy(6+18,8+20);
cprintf("%s",book.bookno);

gotoxy(6+18,8+26);
cprintf("%s",book.issue_to);
gotoxy(6+18,8+28);
cprintf("%s",book.issue_date);
gotoxy(6+18,8+30);
cprintf("%s",book.due_date);
}


void tabview_show(void)
{
textcolor(BLUE);
recno=26; //Exactly fits in the string
for(i=0;i<recno;i++)
 {gotoxy(x+5,y+5+i);
 //cprintf("%s",book.name);
 cprintf("%s","Under construction.");
  }
recno=recold;
}

void bulet(int a,int b)
{
row=b;
if (row>=13 && row <= 13+25)
{
textcolor(BLACK);gotoxy(a,row-1);cprintf("  ");
textcolor(BLACK);gotoxy(a,row+1);cprintf("  ");
textcolor(WHITE);
gotoxy(a,b);cprintf("%s",bullet);
}
else if (row>13+25){textcolor(BLACK);gotoxy(a,13+25);cprintf("  ");
row=13;
textcolor(WHITE);
gotoxy(a,row);cprintf("%s",bullet);
}

}

int read_data(void)
{
	if(maxrec==0)
	{
	rewind(fp);
	while(fread(&book,rec_size,1,fp)==1) //returns nozero on success
	maxrec++;
	}

	rewind(fp);
	fseek(fp,recno*rec_size,SEEK_SET); // Moves file pointer to required RECORD.
	fread(&book,rec_size,1,fp);
return 0;
}
int writehtm(char *htm_file)
{
FILE *fp;
char *nam;
int recold=recno;
sprintf(nam,"c:\\%s.htm",htm_file);
fp=fopen(nam,"w");
fprintf(fp,"<html><head><title>The Library Database</title>");
fprintf(fp,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">");
fprintf(fp,"</head><body bgcolor=\"#CCCCCC\">");
fprintf(fp,"<p align=\"center\"><strong>Thank you for using <font color=\"#0000FF\"><u><br>");
fprintf(fp,"THE LIBRARY</u></font></strong></p>");
fprintf(fp,"<p align=\"center\"><strong><font color=\"#0000FF\"> <font color=\"#FF0000\">");
fprintf(fp,"Created By: THE LIBRARY<br>");
fprintf(fp,"Developed By: SAJEB Group 062/BCT , HCOE,TU<br>");
fprintf(fp,"Samir Thapa<br> Janak Raj Pathak<br> Bhupal Sapkota<br> (C)HCOE, KALOPUL</font></font></strong></p>");
fprintf(fp,"<hr><table width=\"95%\" border=\"1\" align=\"center\">");
fprintf(fp,"<tr>");
fprintf(fp,"<td width=\"4%\"><font color=\"#0000FF\"><strong>S.No</strong></font></td>");
fprintf(fp,"<td width=\"8%\"><font color=\"#0000FF\"><strong>Book No.</strong></font></td>");
fprintf(fp,"<td width=\"27%\"><font color=\"#0000FF\"><strong>Book Name</strong></font></td>");
fprintf(fp,"<td width=\"26%\"><font color=\"#0000FF\"><strong>Author</strong></font></td>");
fprintf(fp,"<td width=\"8%\"><font color=\"#0000FF\"><strong>Edition</strong></font></td>");
fprintf(fp,"<td width=\"27%\"><font color=\"#0000FF\"><strong>ISBN</strong></font></td>");
fprintf(fp,"</tr>");

for(recno=0;recno<=maxrec;recno++)
{
read_data();
fprintf(fp,"<tr>");
fprintf(fp,"<td>%d</td>",recno+1);
	sprintf(tempo,"<td>%s</td>",book.bookno);
fprintf(fp,tempo);
	sprintf(tempo,"<td>%s</td>",book.name);
fprintf(fp,tempo);
	sprintf(tempo,"<td>%s</td>",book.author);
fprintf(fp,tempo);
	sprintf(tempo,"<td>%s</td>",book.edition);
fprintf(fp,tempo);
	sprintf(tempo,"<td>%s</td>",book.isbn);
fprintf(fp,tempo);
fprintf(fp,"</tr>");
}

fprintf(fp,"</table>");
fprintf(fp,"<p align=\"center\"><font color=\"#333333\">&copy;2006, THE LIBRARY</font></p>");
fprintf(fp,"</body>");
fprintf(fp,"</html>");
fclose(fp);
recno=recold;
return 1;
}

int writebook(int editadd)
{
if (editadd==1) // Request to write New records
	{ fseek(fp,0,SEEK_END); // Moves 0 byte from end
	  if(fwrite(&book,rec_size,1,fp)) {fflush(fp);return 1;}
	  else return 0;
	}
if (editadd==2) //Request to write Edited records
	{
	  fseek(fp,-rec_size,SEEK_CUR); //File pointer shifted to the Start of Found Record
	  if(fwrite(&tmp,rec_size,1,fp))
	  {fflush(fp);
	  return 2;}
	  else return 0;
	}
return 0; //Zero return of this function won't interfere with our Output.
/*
strset(book.name,' ');
strset(book.author,' ');
strset(book.pub,' ');
strset(book.edition,' ');
strset(book.isbn,' ');

strset(book.lc,' ');
strset(book.accno,' ');
strset(book.bookno,' ');

strset(book.issue_to,' ');
strset(book.issue_date,' ');
strset(book.due_date,' ');
*/
}

void addbook(int flag)
{
textcolor(WHITE);
input(6+18,8+2,book.name,30);
input(6+18,8+4,book.author,30);
input(6+18,8+6,book.pub,30);
input(6+18,8+8,book.edition,5);
input(6+18,8+10,book.isbn,30);

input(6+18,8+16,book.lc,5);
input(6+18,8+18,book.accno,15);
input(6+18,8+20,book.bookno,15);

input(6+18,8+26,book.issue_to,20);
input(6+18,8+28,book.issue_date,11);
input(6+18,8+30,book.due_date,11);

for(;;)
{
status("IS THE ENTRY CORRECT (Y/N/C):");
	ch=tolower(getch());
	if (ch=='y'|| ch==13)
	{
		if(flag==1)
		  {
		  if((truefalse=writebook(1))==1) //data written to file by call to ADD records
		  {
		  maxrec++;
		  status("Thanks, Book Added.");
		  break;
		  }
		  }
		 else if(flag==2)
		  {
		 if((truefalse=writebook(2))==2) //data written to file by call to EDIT records
		  {
		  status("Thanks, Book Edited.");
		  break;
		  }
		 }
	}
	else if(ch=='n')
		{status("Please re-enter the info.");
		if(flag==1) addbook(1); else if(flag==2) addbook(2);
		}
	else if (ch==27||ch=='c'){recno=recold;status("Entry cancelled!");break;}
	else
		{count++;
		if (count>=3)
		status("It's Not In the choice.");
		}
}//For Loop
}

int searchbook(void)
{
status("Search By <n>Name <a>Author <i>ISBN  <b>Book No");
for(;;)
{
	ch=tolower(getch());
	if (ch=='n'|| ch==13)
	{
		status("");
		gotoxy(15,8+36);textcolor(RED);cprintf("Book Name : ");
		i=input(29,8+36,tempo,30);

		rewind(fp); // moves file pointer to beginning
			while(fread(&book,rec_size,1,fp)==1)
				{
				if(strcmp(book.author,tempo)==0)
				{
				status("");
				clsview();
				oneatime_show();
				return 1;
				}
				else return 0;
				}
	}
	else if(ch=='a')
	{
		status("");
		gotoxy(15,8+36);textcolor(RED);cprintf("Author Name : ");

		i=input(29,8+36,tempo,30);
		rewind(fp); // moves file pointer to beginning
			while(fread(&book,rec_size,1,fp)==1)
				{
				if(strcmp(book.author,tempo)==0)
				{
				status("");
				clsview();
				oneatime_show();
				return 1;
				}
				else return 0;
				}
	}
	else if (ch==27){status(""); status("Search cancelled!");break;}

	else if (ch=='i'|| ch==13)
			{
			status("");
			gotoxy(15,8+36);textcolor(RED);cprintf("Book ISBN : ");
			i=input(28,8+36,tempo,30);

			rewind(fp); // moves file pointer to beginning
			while(fread(&book,rec_size,1,fp)==1)
				{
				if(strcmp(book.isbn,tempo)==0)
				{
				status("");
				clsview();
				oneatime_show();
				return 1;
				}
				else return 0;
				}
	}

	else if (ch=='b'|| ch==13)
		{
		status("");
		gotoxy(15,8+36);textcolor(RED);cprintf("Book No : ");
		i=input(26,8+36,tempo,30);

		rewind(fp); // moves file pointer to beginning
		recold=recno;
		recno=0;
		while(fread(&book,rec_size,1,fp)==1)
			{       recno++;
			if(strcmp(book.bookno,tempo)==0)
			{
			status("");
			clsview();
			oneatime_show();
			return 1;
			}
			else {recno=recold;return 0;}
	}		}
	else {sound(1000);delay(300);nosound();}
}//For Loop
ch='n';
return 0;

}


int editbook(void)
{
char *temp;
strcpy(temp,book.bookno); // Copies current Bookno to temp

rewind(fp); // moves file pointer to beginning
//truefalse=0; //Sets flag to check if record found or not
while(fread(&tmp,rec_size,1,fp)==1)
	{
		if(strcmp(tmp.bookno,temp)==0)
		{
		status("");
		clsview();
		addbook(2);
		return 1;
		}
	}
//if(truefalse==0) //if strcmp didn't found records then comes here
//status("Sorry, book number didn't match with stored one.");
return 0;
}

void flush(FILE *stream)
{
     int duphandle;

     /* flush the stream's internal buffer */
     fflush(stream);

     /* make a duplicate file handle */
     duphandle = dup(fileno(stream));

     /* close the duplicate handle to flush the DOS buffer */
     close(duphandle);
}



void cls(void) //Clears the dISPLAY bOX
{
textcolor(BLACK);
for(j=x-1;j<(x+49);j++)
for(i=y+1;i<y+32;i++)
       {gotoxy(j,i);
	cprintf(" ");
	}
}

void clsview(void) //Clears the fields to print next records
{
textcolor(BLACK);
for (i=y+2;i<=y+10;i=i+2)
      for(j=x+18;j<x+49;j++)
      {gotoxy(j,i);printf(" ");}

for (i=y+16;i<=y+20;i=i+2)
      for(j=x+18;j<x+49;j++)
      {gotoxy(j,i);printf(" ");}

for (i=y+26;i<=y+30;i=i+2)
      for(j=x+18;j<x+49;j++)
      {gotoxy(j,i);printf(" ");}
gotoxy(x+18,y+2); //Sets the cursor to Book Name field
}

void clstab(void) //Clears the 26 items of tabview box
{
textcolor(BLACK);
for(i=0;i<26;i++)
     for(j=x+5;j<=(x+48);j++)
       {gotoxy(j,y+5+i);
	cprintf(" ");
	}
}

void status(char *m) //STATUS AND HELP WINDOW
{
int l=strlen(m);
textcolor(BLACK);for(i=58;i<=78;i++){gotoxy(i,7);cprintf(" ");}
textcolor(RED);gotoxy(58,7);cprintf("%ld",recno+1);
textcolor(BLACK);for(i=5;i<=54;i++){gotoxy(i,8+36);cprintf(" ");}
gotoxy(30-l/2,8+36);textcolor(RED);cprintf("%s",m);
}

void box(int x1,int y1,int x2,int y2,char *text)   //DRAWS BOX
{
char h_border=196,v_border=179;
char tl=218,tr=191,dl=192,dr=217;
int i;
for(i=x1;i<=x2-1;i++)
{gotoxy(i,y1);cprintf("%c",h_border);}
for(i=y1;i<=y2-1;i++)
{gotoxy(x1,i);cprintf("%c",v_border);}
for(i=x1;i<=x2-1;i++)
{gotoxy(i,y2);cprintf("%c",h_border);}
for(i=y1;i<=y2-1;i++)
{gotoxy(x2,i);cprintf("%c",v_border);}
gotoxy(x1,y1);cprintf("%c",tl);
gotoxy(x2,y1);cprintf("%c",tr);
gotoxy(x1,y2);cprintf("%c",dl);
gotoxy(x2,y2);cprintf("%c",dr);
if (strlen(text)>0)
{gotoxy(x1+3,y1);cprintf("%s",text);}
}
