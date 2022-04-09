struct credentials
{
	char userid[10];
	char password[10];
	int accType;
};

struct train_details
{
	char name[20];
	int trainNo;
	int totalSeats;
	int bookedSeats;
};
struct booking_details
{
	int bid;
	int accType;
	char userid[10];
	int trainNo;
	int bookedSeats;
};


