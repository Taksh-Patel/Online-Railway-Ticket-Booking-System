int authenticate(int acc,char uid[],char pass[]);
void operations(int nsd);
void addTrainDetails(int nsd);
void updateTrainDetails(int nsd);
void searchTrainDetails(int nsd);
void deleteTrainDetails(int nsd);
void addAccount(int nsd);
void searchAccount(int nsd);
void updateAccount(int nsd);
void deleteAccount(int nsd);
void displayTrainDetails(int nsd);
void bookTicket(int nsd);
void viewBookings(int nsd);
void updateBookings(int nsd);
void cancelBookings(int nsd);
void agentBookTicket(int nsd);
void agentViewBookings(int nsd);
void agentUpdateBookings(int nsd);
void agentCancelBookings(int nsd);

int authenticate(int acc,char uid[],char pass[])
{	
	int fd,varified=0,r;
	struct flock lock;
	struct credentials cre;
	switch(acc)
	{
		case 1:
			fd = open("normalAccounts.txt",O_RDWR);
			while(!varified)
			{
				r = read(fd,&cre,sizeof(cre));
				if(strcmp(uid,cre.userid)==0 && strcmp(pass,cre.password)==0)
				{
					varified=1;
				}
				if(r==0)
					break;	
			}	
			if(varified==1)
			{
				lock.l_whence = SEEK_CUR;
				lock.l_type = F_WRLCK;
				lock.l_start = -1*sizeof(cre);
				lock.l_len = sizeof(cre);
				lock.l_pid = getpid();
				if(fcntl(fd,F_SETLK,&lock)==-1)
				{
					printf("someone already logged in with same credentials\n");
					return 0;
				}
					
				return 1;
			}
			else
				return 0;
			break;
		case 2:
			fd = open("agentAccounts.txt",O_RDONLY);
			while(!varified)
			{
				r = read(fd,&cre,sizeof(cre));
				if(strcmp(uid,cre.userid)==0 && strcmp(pass,cre.password)==0)
				{
					varified=1;
				}
				if(r==0)
					break;	
			}	
			if(varified==1)				
				return 1;
			else
				return 0;
			break;
		case 3:
			fd = open("adminAccounts.txt",O_RDWR);
			while(!varified)
			{
				r = read(fd,&cre,sizeof(cre));
				if(strcmp(uid,cre.userid)==0 && strcmp(pass,cre.password)==0)
				{
					varified=1;
				}
				if(r==0)
					break;	
			}	
			if(varified==1)
			{
				lock.l_whence = SEEK_CUR;
				lock.l_type = F_WRLCK;
				lock.l_start = -1*sizeof(cre);
				lock.l_len = sizeof(cre);
				lock.l_pid = getpid();
				if(fcntl(fd,F_SETLK,&lock)==-1)
				{
					printf("someone already logged in with same credentials\n");
					return 0;
				}
					
				return 1;
			}
			else
				return 0;
			break;
	}
}
void operations(int nsd)
{
	int auth;
	int op;
	struct credentials cre;
	char success[10] = "success";
	char failure[10] = "failure";
	int acc;
	read(nsd,&cre,sizeof(cre));
	acc = cre.accType;
				
	switch(acc)
	{
		case 1:
			auth = authenticate(acc,cre.userid,cre.password);
			if(auth==1)
			{
				send(nsd,success,sizeof(success),0);
				while(1){
				
					recv(nsd,&op,sizeof(op),0);
					switch(op)
					{
						case 1:
							bookTicket(nsd);
							break;
						case 2:
							viewBookings(nsd);
							break;
						case 3:
							updateBookings(nsd);
							break;
						case 4:
							cancelBookings(nsd);
							break;
						case 5:
							return;
					}
				}			
			}
			else
			{
				send(nsd,failure,sizeof(failure),0);
			}
			break;
		case 2:
			auth = authenticate(acc,cre.userid,cre.password);
			if(auth==1)
			{
				send(nsd,success,sizeof(success),0);
				
				while(1){
					recv(nsd,&op,sizeof(op),0);
					switch(op)
					{
						case 1:
							agentBookTicket(nsd);
							break;
						case 2:
							agentViewBookings(nsd);
							break;
						case 3:
							agentUpdateBookings(nsd);
							break;
						case 4:
							agentCancelBookings(nsd);
							break;
						case 5:
							return;
					}
				}					
			}
			else
			{
				send(nsd,failure,sizeof(failure),0);
			}
			break;
		case 3:
			auth = authenticate(acc,cre.userid,cre.password);
			if(auth==1)
			{
				send(nsd,success,sizeof(success),0);
				
				while(1){
					recv(nsd,&op,sizeof(op),0);
					switch(op)
					{
						case 1:
							addAccount(nsd);
							break;
						case 2:
							deleteAccount(nsd);
							break;
						case 3:
							updateAccount(nsd);
							break;
						case 4:
							searchAccount(nsd);
							break;
						case 5:
							addTrainDetails(nsd);
							break;
						case 6:
							deleteTrainDetails(nsd);
							break;
						case 7:
							updateTrainDetails(nsd);
							break;
						case 8:
							searchTrainDetails(nsd);
							break;
						case 9:
							return;
					}
				}							
			}
			else
			{
				send(nsd,failure,sizeof(failure),0);
			}
			break;
	}
			
}


void addTrainDetails(int nsd)
{	
	int fd,flag=0,r;
	char success[10] = "success";
	char failure[10] = "failure";

	struct train_details tr,tr2;
	recv(nsd,&tr,sizeof(tr),0);
	
	fd = open("train_details.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&tr2,sizeof(tr2));
		if(tr.trainNo==tr2.trainNo)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==0)
	{
		write(fd,&tr,sizeof(tr));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);
}
	
void updateTrainDetails(int nsd)
{
	int fd,flag=0,r;
	char success[10] = "success";
	char failure[10] = "failure";

	struct train_details tr,tr2;
	recv(nsd,&tr,sizeof(tr),0);
	fd = open("train_details.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&tr2,sizeof(tr2));
		if(tr.trainNo==tr2.trainNo)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==1)
	{
		tr.bookedSeats = tr2.bookedSeats;
		lseek(fd,-1*sizeof(tr),SEEK_CUR);
		write(fd,&tr,sizeof(tr));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);
}

void searchTrainDetails(int nsd)
{
	int fd,flag=0,r,trNo;
	char success[10] = "success";
	char failure[10] = "failure";

	struct train_details tr;
	recv(nsd,&trNo,sizeof(trNo),0);
	fd = open("train_details.txt",O_RDONLY);
	while(flag==0)
	{
		r = read(fd,&tr,sizeof(tr));
		if(trNo==tr.trainNo)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==1)
	{
		send(nsd,success,sizeof(success),0);
		send(nsd,&tr,sizeof(tr),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);
}

void deleteTrainDetails(int nsd)
{
	int fd,flag=0,r,trNo;
	char success[10] = "success";
	char failure[10] = "failure";

	struct train_details tr,emt;
	recv(nsd,&trNo,sizeof(trNo),0);
	fd = open("train_details.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&tr,sizeof(tr));
		if(trNo==tr.trainNo)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==1)
	{
		emt.trainNo = -1;
		lseek(fd,-1*sizeof(tr),SEEK_CUR);
		write(fd,&emt,sizeof(emt));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);
}

void addAccount(int nsd)
{
	struct credentials cr1,cr2;
	recv(nsd,&cr1,sizeof(cr1),0);
	int fd,flag=0,r;
	char success[10] = "success";
	char failure[10] = "failure";

	if(cr1.accType==1)
		fd = open("normalAccounts.txt",O_RDWR);
	else if(cr1.accType==2)
		fd = open("agentAccounts.txt",O_RDWR);
	else if(cr1.accType==3)
		fd = open("adminAccounts.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&cr2,sizeof(cr2));
		if(strcmp(cr1.userid,cr2.userid)==0)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==0)
	{
		write(fd,&cr1,sizeof(cr1));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);	
}

void deleteAccount(int nsd)
{
	struct credentials cr1,cr2;
	recv(nsd,&cr1,sizeof(cr1),0);
	int fd,flag=0,r;
	char success[10] = "success";
	char failure[10] = "failure";

	if(cr1.accType==1)
		fd = open("normalAccounts.txt",O_RDWR);
	else if(cr1.accType==2)
		fd = open("agentAccounts.txt",O_RDWR);
	else if(cr1.accType==3)
		fd = open("adminAccounts.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&cr2,sizeof(cr2));
		if(strcmp(cr1.userid,cr2.userid)==0)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==1)
	{
		cr2.accType = -1;
		strcpy(cr2.userid," ");
		strcpy(cr2.password," ");
		lseek(fd,-1*sizeof(cr2),SEEK_CUR);
		write(fd,&cr2,sizeof(cr2));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);	
}
void updateAccount(int nsd)
{
	struct credentials cr1,cr2;
	recv(nsd,&cr1,sizeof(cr1),0);
	int fd,flag=0,r;
	char success[10] = "success";
	char failure[10] = "failure";

	if(cr1.accType==1)
		fd = open("normalAccounts.txt",O_RDWR);
	else if(cr1.accType==2)
		fd = open("agentAccounts.txt",O_RDWR);
	else if(cr1.accType==3)
		fd = open("adminAccounts.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&cr2,sizeof(cr2));
		if(strcmp(cr1.userid,cr2.userid)==0)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==1)
	{
		lseek(fd,-1*sizeof(cr2),SEEK_CUR);
		write(fd,&cr1,sizeof(cr1));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);	

}

void searchAccount(int nsd)
{
	struct credentials cr1,cr2;
	recv(nsd,&cr1,sizeof(cr1),0);
	int fd,flag=0,r;
	char success[10] = "success";
	char failure[10] = "failure";

	if(cr1.accType==1)
		fd = open("normalAccounts.txt",O_RDWR);
	else if(cr1.accType==2)
		fd = open("agentAccounts.txt",O_RDWR);
	else if(cr1.accType==3)
		fd = open("adminAccounts.txt",O_RDWR);
	while(flag==0)
	{
		r = read(fd,&cr2,sizeof(cr2));
		if(strcmp(cr1.userid,cr2.userid)==0)
			flag=1;
		if(r==0)
			break;
	}
	
	if(flag==1)
	{
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);	

}

void displayTrainDetails(int nsd)
{
	struct train_details tr;
	int fd = open("train_details.txt",O_RDWR);
	while(read(fd,&tr,sizeof(tr)))
	{
		send(nsd,&tr,sizeof(tr),0);
	}
}

void bookTicket(int nsd)
{
	displayTrainDetails(nsd);
	char success[10] = "success";
	char failure[10] = "failure";

	int fd,l,fd1,flag=0;
	struct booking_details bd;
	struct train_details tr;	
	recv(nsd,&bd,sizeof(bd),0);
	fd = open("booking_details.txt",O_RDWR);
	fd1 = open("train_details.txt",O_RDWR);
	
	if(bd.bookedSeats>0)
	{
		while(read(fd1,&tr,sizeof(tr)))
		{
			if(bd.trainNo==tr.trainNo && (tr.totalSeats-tr.bookedSeats)>=bd.bookedSeats)
			{
				flag=1;
				break;
			}
		}
	}
	if(flag==1)
	{	
		lseek(fd1,-1*sizeof(tr),SEEK_CUR);
		tr.bookedSeats+=bd.bookedSeats;
		write(fd1,&tr,sizeof(tr));
		l = lseek(fd,0,SEEK_END);
		bd.bid = (l/sizeof(bd))+100;
		write(fd,&bd,sizeof(bd));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);	

}

void viewBookings(int nsd)
{
	char success[10] = "success";
	char failure[10] = "failure";
	char uid[10];
	int fd;
	struct booking_details bd;
	recv(nsd,&uid,sizeof(uid),0);
	fd = open("booking_details.txt",O_RDWR);
	
	while(read(fd,&bd,sizeof(bd)))
	{
		if(strcmp(uid,bd.userid)==0 && bd.accType==1)
		{
			send(nsd,&bd,sizeof(bd),0);
		}
	}
}

void updateBookings(int nsd)
{
	char success[10] = "success";
	char failure[10] = "failure";
	
	int bid,bookedSeats,fd,fd1,diff;
	struct booking_details bd;
	struct train_details tr;
	
	displayTrainDetails(nsd);
	viewBookings(nsd);
	recv(nsd,&bid,sizeof(bid),0);
	recv(nsd,&bookedSeats,sizeof(bookedSeats),0);
	
	fd = open("booking_details.txt",O_RDWR);
	while(read(fd,&bd,sizeof(bd)))
	{
		if(bd.bid==bid)
			break;
	}
	fd1 = open("train_details.txt",O_RDWR);
	while(read(fd1,&tr,sizeof(tr)))
	{
		if(tr.trainNo==bd.trainNo)
			break;
	}
	lseek(fd,-1*sizeof(bd),SEEK_CUR);
	lseek(fd1,-1*sizeof(tr),SEEK_CUR);
	if(bookedSeats<bd.bookedSeats)
	{
		diff = bd.bookedSeats - bookedSeats;
		tr.bookedSeats-=diff;
		bd.bookedSeats = bookedSeats;
		write(fd,&bd,sizeof(bd));
		write(fd1,&tr,sizeof(tr));
		send(nsd,&success,sizeof(success),0);
	}
	else if(bookedSeats>bd.bookedSeats)
	{
		diff = bookedSeats - bd.bookedSeats;
		if((tr.totalSeats - tr.bookedSeats)>=diff)
		{
			tr.bookedSeats+=diff;
			bd.bookedSeats = bookedSeats;
			write(fd,&bd,sizeof(bd));
			write(fd1,&tr,sizeof(tr));
			send(nsd,&success,sizeof(success),0);
		}
		else
			send(nsd,&failure,sizeof(failure),0);
	}
	else
		send(nsd,&failure,sizeof(failure),0);
}

void cancelBookings(int nsd)
{
	char success[10] = "success";
	char failure[10] = "failure";
	
	int bid,fd,fd1;
	struct booking_details bd;
	struct train_details tr;
	
	displayTrainDetails(nsd);
	viewBookings(nsd);
	recv(nsd,&bid,sizeof(bid),0);
	
	fd = open("booking_details.txt",O_RDWR);
	while(read(fd,&bd,sizeof(bd)))
	{
		if(bd.bid==bid)
			break;
	}
	fd1 = open("train_details.txt",O_RDWR);
	while(read(fd1,&tr,sizeof(tr)))
	{
		if(tr.trainNo==bd.trainNo)
			break;
	}
	lseek(fd,-1*sizeof(bd),SEEK_CUR);
	lseek(fd1,-1*sizeof(tr),SEEK_CUR);

	tr.bookedSeats-=bd.bookedSeats;
	bd.bid = -1;
	write(fd,&bd,sizeof(bd));
	write(fd1,&tr,sizeof(tr));
	send(nsd,&success,sizeof(success),0);
}

void agentBookTicket(int nsd)
{
	struct flock lk1,lk2;
	int fd,l,fd1,flag=0;
	char success[10] = "success";
	char failure[10] = "failure";

	struct booking_details bd;
	struct train_details tr;	
	
	fd = open("booking_details.txt",O_RDWR);
	fd1 = open("train_details.txt",O_RDWR);
		
	lk1.l_type = F_WRLCK;
	lk1.l_whence = SEEK_SET;
	lk1.l_start = 0;
	lk1.l_len = 0;
	lk1.l_pid = getpid();

	lk2.l_type = F_WRLCK;
	lk2.l_whence = SEEK_SET;
	lk2.l_start = 0;
	lk2.l_len = 0;
	lk2.l_pid = getpid();
	
	fcntl(fd,F_SETLKW,&lk1);
	fcntl(fd1,F_SETLKW,&lk2);
	
	displayTrainDetails(nsd);
	
	recv(nsd,&bd,sizeof(bd),0);
	
	if(bd.bookedSeats>0)
	{
		while(read(fd1,&tr,sizeof(tr)))
		{
			if(bd.trainNo==tr.trainNo && (tr.totalSeats-tr.bookedSeats)>=bd.bookedSeats)
			{
				flag=1;
				break;
			}
		}
	}
	if(flag==1)
	{	
		lseek(fd1,-1*sizeof(tr),SEEK_CUR);
		tr.bookedSeats+=bd.bookedSeats;
		write(fd1,&tr,sizeof(tr));
		l = lseek(fd,0,SEEK_END);
		bd.bid = (l/sizeof(bd))+100;
		write(fd,&bd,sizeof(bd));
		send(nsd,success,sizeof(success),0);
	}
	else
		send(nsd,failure,sizeof(failure),0);	
	
	lk1.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lk1);
	lk2.l_type = F_UNLCK;
	fcntl(fd1,F_SETLK,&lk2);
}

void agentViewBookings(int nsd)
{
	struct flock lk;
	char success[10] = "success";
	char failure[10] = "failure";
	char uid[10];
	int fd,found=0;
	struct booking_details bd;
	
	lk.l_type = F_RDLCK;
	lk.l_whence = SEEK_SET;
	lk.l_start = 0;
	lk.l_len = 0;
	lk.l_pid = getpid();
	
	recv(nsd,&uid,sizeof(uid),0);
	fd = open("booking_details.txt",O_RDWR);
	fcntl(fd,F_SETLKW,&lk);
	
	while(read(fd,&bd,sizeof(bd)))
	{
		if(strcmp(uid,bd.userid)==0 && bd.accType==2)
		{
			send(nsd,&bd,sizeof(bd),0);
			found=1;
		}
	}
	if(found==0)
	{
		bd.bid=-1;
		send(nsd,&bd,sizeof(bd),0);
	}
	lk.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lk);
}

void agentUpdateBookings(int nsd)
{
	struct flock lk1,lk2;
	char success[10] = "success";
	char failure[10] = "failure";
	
	int bid,bookedSeats,fd,fd1,diff;
	struct booking_details bd;
	struct train_details tr;
	
	lk1.l_type = F_WRLCK;
	lk1.l_whence = SEEK_SET;
	lk1.l_start = 0;
	lk1.l_len = 0;
	lk1.l_pid = getpid();

	lk2.l_type = F_WRLCK;
	lk2.l_whence = SEEK_SET;
	lk2.l_start = 0;
	lk2.l_len = 0;
	lk2.l_pid = getpid();
	
	fd = open("booking_details.txt",O_RDWR);
	fd1 = open("train_details.txt",O_RDWR);
	fcntl(fd,F_SETLKW,&lk1);
	fcntl(fd1,F_SETLKW,&lk2);
	
	displayTrainDetails(nsd);
	agentViewBookings(nsd);
	recv(nsd,&bid,sizeof(bid),0);
	if(bid==-1)
	{
		goto terminate;
	}
	
	recv(nsd,&bookedSeats,sizeof(bookedSeats),0);
	
	while(read(fd,&bd,sizeof(bd)))
	{
		if(bd.bid==bid)
			break;
	}
	while(read(fd1,&tr,sizeof(tr)))
	{
		if(tr.trainNo==bd.trainNo)
			break;
	}
	lseek(fd,-1*sizeof(bd),SEEK_CUR);
	lseek(fd1,-1*sizeof(tr),SEEK_CUR);
	if(bookedSeats<bd.bookedSeats)
	{
		diff = bd.bookedSeats - bookedSeats;
		tr.bookedSeats-=diff;
		bd.bookedSeats = bookedSeats;
		write(fd,&bd,sizeof(bd));
		write(fd1,&tr,sizeof(tr));
		send(nsd,&success,sizeof(success),0);
	}
	else if(bookedSeats>bd.bookedSeats)
	{
		diff = bookedSeats - bd.bookedSeats;
		if((tr.totalSeats - tr.bookedSeats)>=diff)
		{
			tr.bookedSeats+=diff;
			bd.bookedSeats = bookedSeats;
			write(fd,&bd,sizeof(bd));
			write(fd1,&tr,sizeof(tr));
			send(nsd,&success,sizeof(success),0);
		}
		else
			send(nsd,&failure,sizeof(failure),0);
	}
	else
		send(nsd,&failure,sizeof(failure),0);
	
	terminate:
	lk1.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lk1);
	lk2.l_type = F_UNLCK;
	fcntl(fd1,F_SETLK,&lk2);
}

void agentCancelBookings(int nsd)
{
	struct flock lk1,lk2;
	char success[10] = "success";
	char failure[10] = "failure";
	
	int bid,fd,fd1;
	struct booking_details bd;
	struct train_details tr;
	
	lk1.l_type = F_WRLCK;
	lk1.l_whence = SEEK_SET;
	lk1.l_start = 0;
	lk1.l_len = 0;
	lk1.l_pid = getpid();

	lk2.l_type = F_WRLCK;
	lk2.l_whence = SEEK_SET;
	lk2.l_start = 0;
	lk2.l_len = 0;
	lk2.l_pid = getpid();
	
	fd = open("booking_details.txt",O_RDWR);
	fd1 = open("train_details.txt",O_RDWR);
	fcntl(fd,F_SETLKW,&lk1);
	fcntl(fd1,F_SETLKW,&lk2);
	
	displayTrainDetails(nsd);
	agentViewBookings(nsd);
	recv(nsd,&bid,sizeof(bid),0);
	if(bid==-1)
	{
		goto terminate;
	}
	while(read(fd,&bd,sizeof(bd)))
	{
		if(bd.bid==bid)
			break;
	}

	while(read(fd1,&tr,sizeof(tr)))
	{
		if(tr.trainNo==bd.trainNo)
			break;
	}
	lseek(fd,-1*sizeof(bd),SEEK_CUR);
	lseek(fd1,-1*sizeof(tr),SEEK_CUR);

	tr.bookedSeats-=bd.bookedSeats;
	bd.bid = -1;
	write(fd,&bd,sizeof(bd));
	write(fd1,&tr,sizeof(tr));
	send(nsd,&success,sizeof(success),0);
	
	terminate:
	lk1.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lk1);
	lk2.l_type = F_UNLCK;
	fcntl(fd1,F_SETLK,&lk2);
}

