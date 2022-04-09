void mainMenu(int sd);
void normalAccountMenu(int sd,char uid[]);
void agentAccountMenu(int sd,char uid[]);
void adminAccountMenu(int sd,char uid[]);

void mainMenu(int sd)
{
	struct credentials cre;
	int acc;
	char uid[10];
	char pass[10];
	char status[10];
	char success[10] = "success";
	
	printf("Please Login\n");
	printf("1. Normal Account\n");
	printf("2. Agent Account\n");
	printf("3. Administrator Account\n");
	
	printf("\n");
	printf("Enter your Account Type\n");
	scanf("%d",&acc);
	printf("User ID: ");
	scanf("%s",uid);
	printf("Password: ");
	scanf("%s",pass);
	printf("\n");
	
	cre.accType = acc;
	strcpy(cre.userid,uid);
	strcpy(cre.password,pass);
	
	send(sd,&cre,sizeof(cre),0);
	recv(sd,&status,sizeof(status),0);
	if(strcmp(success,status)==0)
	{
		switch(acc)
		{
			case 1:
				printf("successfully logged in Normal account\n");
				normalAccountMenu(sd,uid);
				break;
			case 2:
				printf("successfully logged in Agent account\n");
				agentAccountMenu(sd,uid);
				break;
			case 3:
				printf("successfully logged in Admin account\n");
				adminAccountMenu(sd,uid);
				break;
		}
	}
	else
	{
		printf("login failed!\n");
	}
}

void normalAccountMenu(int sd,char uid[])
{	
	int op,bid,bookedSeats,found;
	struct booking_details bd;
	struct train_details tr;
	char status[10];
	char success[10] = "success";

	while(1){	
		printf("\n**********************************************************************************************************\n");
		printf("\n--------------------------------\n");
		printf("1. Book Ticket\n");
		printf("2. View Previous Bookings\n");
		printf("3. Update Booking\n");
		printf("4. Cancel Booking\n");
		printf("5. Exit\n");
		printf("--------------------------------\n");
		printf("choose operation\n");
		scanf("%d",&op);
		found=0;
		switch(op)
		{
			case 1:
				printf("\n");
				send(sd,&op,sizeof(op),0);
				printf("train details\n");
				printf("           Train Name|Train Number|Total Seats|Booked Seats|Available Seats\n"); 
				sleep(2);  
				while(recv(sd,&tr,sizeof(tr),MSG_DONTWAIT)>0)
				{
					if(tr.trainNo!=-1)
					{
						printf("%20s ",tr.name);
						printf("%10d ",tr.trainNo);
						printf("%10d ",tr.totalSeats);
						printf("%10d ",tr.bookedSeats);
						printf("%10d \n",tr.totalSeats-tr.bookedSeats);
					}
				}
				bd.accType = 1;
				strcpy(bd.userid,uid);
				printf("Enter train number for booking : ");
				scanf("%d",&bd.trainNo);
				printf("Enter Number of seats you want to book : ");
				scanf("%d",&bd.bookedSeats);
				send(sd,&bd,sizeof(bd),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Tickets Booked\n");
				}
				else
					printf("booking failed\n");
				break;
			case 2:
				printf("\n");
				printf("Previous Bookings \n");
				send(sd,&op,sizeof(op),0);
				send(sd,uid,10,0);
				
				printf("Booking ID | Account Type | User ID | Train Number | Booked Seats\n");
				sleep(2);
				while(recv(sd,&bd,sizeof(bd),MSG_DONTWAIT)>0)
				{
					if(bd.trainNo!=-1 && bd.bid!=-1)
					{
						printf("%10d",bd.bid);
						printf("%10d",bd.accType);
						printf("%15s",bd.userid);
						printf("%10d",bd.trainNo);
						printf("%10d\n",bd.bookedSeats);
					}
				}
				break;			
			case 3:
				printf("\n");
				send(sd,&op,sizeof(op),0);
				printf("train details\n");
				printf("           Train Name|Train Number|Total Seats|Booked Seats|Available Seats\n"); 
				sleep(2);  
				while(recv(sd,&tr,sizeof(tr),MSG_DONTWAIT)>0)
				{
					if(tr.trainNo!=-1)
					{
						printf("%20s ",tr.name);
						printf("%10d ",tr.trainNo);
						printf("%10d ",tr.totalSeats);
						printf("%10d ",tr.bookedSeats);
						printf("%10d \n",tr.totalSeats-tr.bookedSeats);
					}
				}
				printf("\nPrevious Bookings \n");
				send(sd,uid,10,0);
				
				printf("Booking ID | Account Type | User ID | Train Number | Booked Seats\n");
				sleep(2);
				while(recv(sd,&bd,sizeof(bd),MSG_DONTWAIT)>0)
				{
					if(bd.trainNo!=-1 && bd.bid!=-1)
					{
						found=1;
						printf("%10d",bd.bid);
						printf("%10d",bd.accType);
						printf("%15s",bd.userid);
						printf("%10d",bd.trainNo);
						printf("%10d\n",bd.bookedSeats);
					}
				}
				if(found==0)
				{
					printf("\nNo records to update\n");
					break;
				}
				printf("\nupdate booking\n");
				printf("Enter Booking ID(bid) to update number of seats to book: ");
				scanf("%d",&bid);
				printf("Updated Number of seats: ");
				scanf("%d",&bookedSeats);
				send(sd,&bid,sizeof(bid),0);
				send(sd,&bookedSeats,sizeof(bookedSeats),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Tickets Booking Updated\n");
				}
				else
					printf("booking updation failed\n");
				break;

			case 4:
				printf("\n");
				send(sd,&op,sizeof(op),0);
				printf("train details\n");
				printf("           Train Name|Train Number|Total Seats|Booked Seats|Available Seats\n");
				sleep(2);  
				while(recv(sd,&tr,sizeof(tr),MSG_DONTWAIT)>0)
				{
					if(tr.trainNo!=-1)
					{
						printf("%20s ",tr.name);
						printf("%10d ",tr.trainNo);
						printf("%10d ",tr.totalSeats);
						printf("%10d ",tr.bookedSeats);
						printf("%10d \n",tr.totalSeats-tr.bookedSeats);
					}
				}
				printf("\nPrevious Bookings \n");
				send(sd,uid,10,0);
				
				printf("Booking ID | Account Type | User ID | Train Number | Booked Seats\n");
				sleep(2);
				while(recv(sd,&bd,sizeof(bd),MSG_DONTWAIT)>0)
				{
					if(bd.trainNo!=-1 && bd.bid!=-1)
					{
						found=1;
						printf("%10d",bd.bid);
						printf("%10d",bd.accType);
						printf("%15s",bd.userid);
						printf("%10d",bd.trainNo);
						printf("%10d\n",bd.bookedSeats);
					}
				}
				if(found==0)
				{
					printf("\nNo records to cancel\n");
					break;
				}
				printf("\nCancel booking\n");
				printf("Enter Booking ID(bid) of booking which you want to Cancel: ");
				scanf("%d",&bid);
				send(sd,&bid,sizeof(bid),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Tickets Booking Cancelled\n");
				}
				else
					printf("booking Cancellation failed\n");
				break;
			case 5:
				send(sd,&op,sizeof(op),0);
				exit(0);
				break;
		}
	}
		
}

void agentAccountMenu(int sd,char uid[])
{
	int op,bid,bookedSeats,found;
	struct booking_details bd;
	struct train_details tr;
	char status[10];
	char success[10] = "success";
	
	while(1){
		printf("\n**********************************************************************************************************\n");
		printf("\n--------------------------------\n");
		printf("1. Book Ticket\n");
		printf("2. View Previous Bookings\n");
		printf("3. Update Booking\n");
		printf("4. Cancel Booking\n");
		printf("5. Exit\n");
		printf("--------------------------------\n");
		printf("choose operation\n");
		scanf("%d",&op);
		found=0;
		switch(op)
		{
			case 1:
				printf("\n");
				send(sd,&op,sizeof(op),0);
				printf("train details\n");
				printf("           Train Name|Train Number|Total Seats|Booked Seats|Available Seats\n");
				
				recv(sd,&tr,sizeof(tr),0);
				do{
					if(tr.trainNo!=-1)
					{
						printf("%20s ",tr.name);
						printf("%10d ",tr.trainNo);
						printf("%10d ",tr.totalSeats);
						printf("%10d ",tr.bookedSeats);
						printf("%10d \n",tr.totalSeats-tr.bookedSeats);
					}
				}while(recv(sd,&tr,sizeof(tr),MSG_DONTWAIT)>0);
				
				bd.accType = 2;
				strcpy(bd.userid,uid);
				printf("Enter train number for booking : ");
				scanf("%d",&bd.trainNo);
				printf("Enter Number of seats you want to book : ");
				scanf("%d",&bd.bookedSeats);
				send(sd,&bd,sizeof(bd),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Tickets Booked\n");
				}
				else
					printf("booking failed\n");
				break;
			case 2:
				printf("\n");
				printf("Previous Bookings \n");
				send(sd,&op,sizeof(op),0);
				send(sd,uid,10,0);
				
				printf("Booking ID | Account Type | User ID | Train Number | Booked Seats\n");
				recv(sd,&bd,sizeof(bd),0);
				do{
					if(bd.trainNo!=-1 && bd.bid!=-1)
					{
						printf("%10d",bd.bid);
						printf("%10d",bd.accType);
						printf("%15s",bd.userid);
						printf("%10d",bd.trainNo);
						printf("%10d\n",bd.bookedSeats);
					}
				}while(recv(sd,&bd,sizeof(bd),MSG_DONTWAIT)>0);
				break;			
			case 3:
				printf("\n");
				send(sd,&op,sizeof(op),0);
				printf("train details\n");
				printf("           Train Name|Train Number|Total Seats|Booked Seats|Available Seats\n");
				recv(sd,&tr,sizeof(tr),0);
				do{
					if(tr.trainNo!=-1)
					{
						printf("%20s ",tr.name);
						printf("%10d ",tr.trainNo);
						printf("%10d ",tr.totalSeats);
						printf("%10d ",tr.bookedSeats);
						printf("%10d \n",tr.totalSeats-tr.bookedSeats);
					}
				}while(recv(sd,&tr,sizeof(tr),MSG_DONTWAIT)>0);
				printf("\nPrevious Bookings \n");
				send(sd,uid,10,0);
				
				printf("Booking ID | Account Type | User ID | Train Number | Booked Seats\n");
				recv(sd,&bd,sizeof(bd),0);
				do{
					if(bd.trainNo!=-1 && bd.bid!=-1)
					{
						found=1;
						printf("%10d",bd.bid);
						printf("%10d",bd.accType);
						printf("%15s",bd.userid);
						printf("%10d",bd.trainNo);
						printf("%10d\n",bd.bookedSeats);
					}
					sleep(1);
				}while(recv(sd,&bd,sizeof(bd),MSG_DONTWAIT)>0);
				
				if(found==0)
				{
					bid=-1;
					send(sd,&bid,sizeof(bid),0);
					printf("\nNo records to update\n");
					break;
				}
				printf("\nupdate booking\n");
				printf("Enter Booking ID(bid) to update number of seats to book: ");
				scanf("%d",&bid);
				printf("Updated Number of seats: ");
				scanf("%d",&bookedSeats);
				send(sd,&bid,sizeof(bid),0);
				send(sd,&bookedSeats,sizeof(bookedSeats),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Tickets Booking Updated\n");
				}
				else
					printf("booking updation failed\n");
				break;

			case 4:
				printf("\n");
				send(sd,&op,sizeof(op),0);
				printf("train details\n");
				printf("           Train Name|Train Number|Total Seats|Booked Seats|Available Seats\n");
				recv(sd,&tr,sizeof(tr),0);
				do{
					if(tr.trainNo!=-1)
					{
						printf("%20s ",tr.name);
						printf("%10d ",tr.trainNo);
						printf("%10d ",tr.totalSeats);
						printf("%10d ",tr.bookedSeats);
						printf("%10d \n",tr.totalSeats-tr.bookedSeats);
					}
				}while(recv(sd,&tr,sizeof(tr),MSG_DONTWAIT)>0);
				printf("\nPrevious Bookings \n");
				send(sd,uid,10,0);
				
				printf("Booking ID | Account Type | User ID | Train Number | Booked Seats\n");
				recv(sd,&bd,sizeof(bd),0);
				do{
					if(bd.trainNo!=-1 && bd.bid!=-1)
					{
						found=1;
						printf("%10d",bd.bid);
						printf("%10d",bd.accType);
						printf("%15s",bd.userid);
						printf("%10d",bd.trainNo);
						printf("%10d\n",bd.bookedSeats);
					}
				}while(recv(sd,&bd,sizeof(bd),MSG_DONTWAIT)>0);
				
				if(found==0)
				{
					bid=-1;
					send(sd,&bid,sizeof(bid),0);
					printf("\nNo records to cancel\n");
					break;
				}
				printf("\nCancel booking\n");
				printf("Enter Booking ID(bid) of booking which you want to Cancel: ");
				scanf("%d",&bid);
				send(sd,&bid,sizeof(bid),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Tickets Booking Cancelled\n");
				}
				else
					printf("booking Cancellation failed\n");
				break;
			case 5:
				send(sd,&op,sizeof(op),0);
				exit(0);
				break;
		}
	}
}

void adminAccountMenu(int sd,char uid[])
{
	int op;
	int trNo;
	struct train_details tr;
	struct credentials cr;
	char status[10];
	char success[10] = "success";
	
	while(1){
		printf("\n**********************************************************************************************************\n");
		printf("\n--------------------------------\n");
		printf("1. Add Account\n");
		printf("2. Delete Account\n");
		printf("3. Update Account\n");
		printf("4. Search Account\n");
		printf("5. Add Train Details\n");
		printf("6. Delete Train Details\n");
		printf("7. Update Train Details\n");
		printf("8. Search Train Details\n");
		printf("9. Exit\n");
		printf("--------------------------------\n");	
		printf("choose operation\n");
		scanf("%d",&op);
		switch(op)
		{
			case 1:
				printf("\n");
				printf("Account type : ");
				scanf("%d",&cr.accType);
				printf("User ID : ");
				scanf("%s",cr.userid);
				printf("Password : ");
				scanf("%s",cr.password);
				send(sd,&op,sizeof(op),0);
				send(sd,&cr,sizeof(cr),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("New Account added successfully\n");
				}
				else
					printf("Account already Exist\n");
				break;
			case 2:
				printf("\n");
				printf("Account type : ");
				scanf("%d",&cr.accType);
				printf("User ID : ");
				scanf("%s",cr.userid);
				printf("Password : ");
				scanf("%s",cr.password);
				send(sd,&op,sizeof(op),0);
				send(sd,&cr,sizeof(cr),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Account deleted successfully\n");
				}
				else
					printf("Account Not Exist\n");
				break;
			case 3:
				printf("\n");
				printf("Account type : ");
				scanf("%d",&cr.accType);
				printf("User ID : ");
				scanf("%s",cr.userid);
				printf("Updated Password : ");
				scanf("%s",cr.password);
				send(sd,&op,sizeof(op),0);
				send(sd,&cr,sizeof(cr),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Account updated successfully\n");
				}
				else
					printf("Account Not Exist\n");
				break;
			case 4:
				printf("\n");
				printf("Account type : ");
				scanf("%d",&cr.accType);
				printf("User ID : ");
				scanf("%s",cr.userid);
				send(sd,&op,sizeof(op),0);
				send(sd,&cr,sizeof(cr),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Account Found\n");
				}
				else
					printf("Account Not Found\n");
				break;
			case 5:
				printf("\n");
				printf("Train name: ");
				scanf("%s",tr.name);
				printf("Train number: ");
				scanf("%d",&tr.trainNo);
				printf("No of total seats: ");
				scanf("%d",&tr.totalSeats);
				tr.bookedSeats = 0;
				send(sd,&op,sizeof(op),0);
				send(sd,&tr,sizeof(tr),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Train Details added successfully\n");
				}
				else
					printf("Train Details already exists\n");
				break;
			case 6:
				printf("\n");
				printf("Enter train number which details you want to Delete: ");
				scanf("%d",&trNo);
				send(sd,&op,sizeof(op),0);
				send(sd,&trNo,sizeof(trNo),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Train Details deleted successfully\n");
				}
				else
					printf("Train Details Not exists\n");
				break;
			case 7:
				printf("\n");
				printf("Enter train number of the train which details you want to update: ");
				scanf("%d",&tr.trainNo);
				printf("Enter updated train name: ");
				scanf("%s",tr.name);
				printf("Enter updated no. of total seats: ");
				scanf("%d",&tr.totalSeats);
				send(sd,&op,sizeof(op),0);
				send(sd,&tr,sizeof(tr),0);
				recv(sd,&status,sizeof(status),0);
				if(strcmp(status,success)==0)
				{
					printf("Train Details updated successfully\n");
				}
				else
					printf("Train Details not exists\n");
				break;
				
			case 8:
				printf("\n");
				printf("Enter Train number of train which details you want to search: ");
				scanf("%d",&trNo);
				send(sd,&op,sizeof(op),0);
				send(sd,&trNo,sizeof(trNo),0);
				recv(sd,&status,sizeof(status),0);
				
				if(strcmp(status,success)==0)
				{
					recv(sd,&tr,sizeof(tr),0);
					printf("Train Details\n");
					printf("Train name : %s\n",tr.name);
					printf("Train number : %d\n",tr.trainNo);
					printf("Total seats : %d\n",tr.totalSeats);
					printf("Booked seats : %d\n",tr.bookedSeats);
					printf("Available seats : %d\n",tr.totalSeats-tr.bookedSeats);
				}
				else
					printf("Train Details not exists\n");
				break;
			case 9:
				send(sd,&op,sizeof(op),0);
				exit(0);
				break;
		}
	}
}
