#include<reg51.h>

sbit K1_up = P3^2;
sbit K2_down = P3^3;

#define MAX_LEN  17
#define MIN_LEN  1

unsigned char body_len = 1;
unsigned char head = 0;
unsigned char state = 0;

unsigned char Pos[] = {0,1,2,3,4,5,6,7,7,
					   7,6,5,4,3,2,1,0,0,
                       0,1,2,3,4,5,6,7,7,
                       7,6,5,4,3,2,1,0,0};
unsigned char Seg[] = {0,0,0,0,0,0,0,0,5,
                       6,6,6,6,6,6,6,6,2,
                       3,3,3,3,3,3,3,3,4,
                       6,6,6,6,6,6,6,6,1};
unsigned char _Seg[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char Pos_seg[] = {0,0,0,0,0,0,0,0};
	
void Zero(void)
{
	unsigned char i=0;
	for(i;i<=7;i++)
	{
		Pos_seg[i] = 0x00;
	}
	return;
}

void move(void)
{
	if(head==35){
		head=0;
	}
	else{
		head++;
	}
	return;
}

unsigned char pos_to_P2(unsigned char pos)
{
	return pos<<2;
}

void load(int index)
{
	Pos_seg[ Pos[index] ] |= _Seg[ Seg[index] ];
	return;
}

void refresh(void)
{
    int i;

    Zero();

    i = head;
    if (head >= body_len - 1) {
        for (; i >= head - body_len + 1; i--)
            load(i);
    } else {
        for (; i >= 0; i--)  
            load(i);
        for (i = 35; i >= 37 + head - body_len; i--)
            load(i);
    }
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = ms; i > 0; i--)
        for (j = 120; j > 0; j--);
	return;
}

void scan(void)
{
	if(K1_up==0 && body_len<MAX_LEN)
	{
		body_len++;
		state = 1;
	}
	else if(K2_down==0 && body_len>MIN_LEN)
	{
		body_len--;
		state = 1;
	}
	return;
}

void show(void)
{
	unsigned char i=0,j=0;
	for(;i<40;i++)
	{
		if(state==0){scan();}
		refresh();
		j=0;
		for(;j<=7;j++)
		{
			P2 = pos_to_P2(j);
			P0 = Pos_seg[j];
			delay_ms(1);
		}
	}
	return;
}



void  main(void)
{
	while(1)
	{
		state=0;
		show();
		move();
	}
}