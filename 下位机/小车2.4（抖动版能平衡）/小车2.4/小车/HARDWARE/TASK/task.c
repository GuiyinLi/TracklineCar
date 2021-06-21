#include "task.h"
//#include "main.c"

static int i =0,j=0,k=0,l=0,m=0,n=0;


 void display_task(double angle,u8 y)
{
			short temp = angle;
			if(temp<0)
				{
					OLED_ShowString(30,y,"-");
					temp = -temp;
				}else OLED_ShowString(30,y," ");
			OLED_ShowNum(55,y,temp,5,16);
}

void stop()
{
	    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}	
			if(mode==0)display_task(dis_x,3);
			//display_task(dis_y,6);
	    display_task(yaw,0);
			speed1 = 0;//right
			speed2 = 0;//lift
}

void lift1()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=155;//85
	speed2=135;//70
}

void right1()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=135;//72
	speed2=140;//80
}
void straight1()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=100;
	speed2=100;
}
void task1()
{
	 straight1();	
   if(dis_y>=6000)//&&dis_x<=20
	 {
		 led=!led; 
		 while(1)stop();			 
	 }	 
	  if(yaw>2)	right1();
	  else if(yaw<-1) 	lift1();//
    else if(dis_x<-1) right1();//
    else if(dis_x>1)	lift1();//
    else straight1();// 
 }

void lift2()
{
  in1=1;
	in2=0;
	in3=0;
	in4=1;
	speed1=55;  //r
	speed2=20;  //l
}


void task2()
{
	 straight1();	
   if(dis_y>=6000||i==1)
	 {
		 led=!led; 
     while(yaw<90&&i==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 if(i==1)break;
				 q=1,p=1;
				 lift2();
				 if(yaw>=90)
				 {
					 p=0,q=0,i=1;
					 break;
				 }
			 }
	   if(dis_x<=-6000)
			 {
				 led=!led; 
				 while(1)stop();		 
			 }
		if(yaw>92)	right1();
	  else if(yaw<90) 	lift1();//
    else if(dis_y<5998) right1();//
    else if(dis_y>6001)	lift1();//
    else straight1();// 			 
	 }
	 if(i==0)
	 {
		if(yaw>2)	right1();
	  else if(yaw<-1) 	lift1();//
    else if(dis_x<-1) right1();//
    else if(dis_x>1)	lift1();//
    else straight1();// 
	 }
}
void right3()
{
  in1=0;
	in2=1;
	in3=1;
	in4=0;
	speed1=20;  //r
	speed2=55;  //l
}

void line1()
{
	if(dis_y>=2000||i==1)
	 {
		 led=!led; 
     while(yaw>-90&&i==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 if(i==1)break;
				 q=1,p=1;
				 right3();
				 if(yaw<=-90)
					 {
						 p=0,q=0,i=1;
						 break;
					 }				 
			 }
			 line2();
		 }
	 if(i==0)
	 {
		 if(dis_x<-2)	right1();
		 if(dis_x>2) 	lift1();
		 if(yaw>1) 		right1();		 	   
		 if(yaw<-1) 	lift1();
	 }
}
	 
void line2()
{
	if(dis_x>=2000||j==1)
		 {
			 led=!led; 
			 while(yaw<0&&j==0)	
				 {
					 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
					 q=1,p=1;
					 lift2();
					 if(yaw>=0)
						 {
							 p=0,q=0,j=1;
							 break;
						 }
				}
				line3();
			}
	if(j==0)
		{		
		 if(dis_y<1998)	  lift1();
		 if(dis_y>2002) 	right1();
		 if(yaw>-89) 			right1();		 	   
		 if(yaw<-91) 			lift1();	
		}	
}

void line3()
{
 if(dis_y>=4000||k==1)
	 {
		 led=!led; 
		 while(yaw<90&&k==0)	
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 lift2();
				 if(yaw>=90)
					 {
						 p=0,q=0,k=1;
						 break;
					 }					 					 
			}
		line4();				 
	}
 if(k==0)
	 {
		 if(dis_x<1998)	  right1();
		 if(dis_x>2002) 	lift1();
		 if(yaw>1) 				right1();		 	   
		 if(yaw<-1) 			lift1();			 			 
	 }
}

void line4()
{
		if(dis_x<=0||l==1)
		 {
			 led=!led;
			 while(yaw>0&&l==0)
				{
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 right3();
				if(yaw<=0)
				 {
					 p=0,q=0,l=1;
					 break;										 									 
				 }										 
			 }
			line5();
		 }
	if(l==0)
		{
		 if(dis_y<3998)	  right1();
		 if(dis_y>4002) 	lift1();
		 if(yaw>91) 			right1();		 	   
		 if(yaw<89) 			lift1();					
		}	
}
	
void line5()
{
	if(dis_y>=6000||m==1)
		{
			 led=!led;
			 while(yaw>-90&&m==0)
				{
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 right3();
				if(yaw<=-90)
				 {
					 p=0,q=0,m=1;
					 break;										 									 
				 }
			 }
			line6();
		}
 if(m==0)
	 {
		 if(dis_x<-2)	right1();
		 if(dis_x>2) 	lift1();
		 if(yaw>1) 		right1();		 	   
		 if(yaw<-1) 	lift1();										 
	 }
}

void line6()
{
		if(dis_x>=6000)
			{
				led=!led;
				while(1)stop();																												
			}
		if(n==0)
		{

		 if(dis_y<5998)	  lift1();
		 if(dis_y>6002) 	right1();
		 if(yaw>-89) 			right1();		 	   
		 if(yaw<-91) 			lift1();														
		}
}
void line_straight()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=120;
	speed2=120;
}
void task3()
{
	 line_straight();	
	 line1();
}

void right_arc1()
{
	in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=78;  //r
	speed2=83;  //l
}
void arc1_lift()
{
  in1=1;
	in2=0;
	in3=0;
	in4=1;
	speed1=35;  //r
	speed2=20;  //l
}
void Arc1()
{
	if((dis_x>=980)||i==1)//dis_y>=2000&&
	 {
		 led=!led; 
     while(yaw<-60&&i==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 arc1_lift();
				 if(yaw>=-60)
					 {
						 p=0,q=0,i=1;
						 break;
					 }				 
			 }
			 Arc2();
		 }
	 if(i==0)
	 {
			right_arc1();
	 }
}

void lift_arc2()
{
	in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=83;  //r
	speed2=67;  //l
}
void arc2_lift()
{
  in1=1;
	in2=0;
	in3=0;
	in4=1;
	speed1=50;  //r
	speed2=20;  //l
}
void arc2_right()
{
  in1=0;
	in2=1;
	in3=1;
	in4=0;
	speed1=55;  //r
	speed2=20;  //l
}

void Arc2()
{
	if((dis_y>=4000)||j==1)
	 {
		 led=!led; 
     while(yaw<90&&j==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 arc2_lift();
				 if(yaw>=90)
					 {
						 p=0,q=0,j=1;
						 break;
					 }				 
			 }
     while(yaw>90&&j==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 arc2_right();
				 if(yaw<=90)
					 {
						 p=0,q=0,j=1;
						 break;
					 }				 
			 }
			 Arc3();
		 }
	 if(j==0)
	 {
			lift_arc2();
	 }
}	

void right_arc3()
{
	in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=72;  //r
	speed2=85;  //l
}	

void arc3_lift()
{
  in1=1;
	in2=0;
	in3=0;
	in4=1;
	speed1=45;  //r
	speed2=20;  //l

}
void arc3_right()
{
  in1=0;
	in2=1;
	in3=1;
	in4=0;
	speed1=45;  //r
	speed2=20;  //l

}
void Arc3()
{
	if((dis_y>=4700)||k==1)//&&dis_x==0
	 {
		 led=!led; 
     while(yaw<0&&k==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 arc3_lift();
				 if(yaw>=0)
					 {
						 p=0,q=0,k=1;
						 break;
					 }				 
			 }
     while(yaw>0&&k==0)
			 {
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 arc3_right();
				 if(yaw<=0)
					 {
						 p=0,q=0,k=1;
						 break;
					 }				 
			 }
			 Arc4();
		 }
	 if(k==0)
	 {
			right_arc3();
	 }
}	

void arc4_right()
{
  in1=0;
	in2=1;
	in3=1;
	in4=0;
	speed1=20;  //r
	speed2=42;  //l
}

void arc4_straight()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=120;
	speed2=120;
}

void arc4_right1()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=85;//72
	speed2=90;//80
}

void arc_lift1()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=100;//85
	speed2=85;//70
}

void Arc4()
{
		if(dis_y>=6000||l==1)
			{
				led=!led;
				while(yaw>-90&&l==0)
				{
				 if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}				 					 
				 q=1,p=1;
				 arc4_right();
				 if(yaw<=-90)
					 {
						 p=0,q=0,l=1;
						 break;
					 }			
				}
			Arc5();
			}
			
		if(l==0)
		{
			if(yaw>1)						arc4_right1();
			else if(yaw<-1) 		arc_lift1();//
			else if(dis_x>1) 		arc_lift1();//
			else if(dis_x<-1)		arc4_right1();//
			else arc4_straight();// 			
		}
}
void arc5_straight()
{
  in1=1;
	in2=0;
	in3=1;
	in4=0;
	speed1=100;
	speed2=100;
}
void Arc5()
{
		if(dis_x>=6000)
			{
				led=!led;
				while(1)stop();																												
			}
	  if(yaw>-89)	right1();
	  else if(yaw<-91) 	lift1();//
    else if(dis_y>6002) right1();//
    else if(dis_y<5998)	lift1();//
    else arc5_straight();// 			
}	    

void task4()
{
	Arc1();
}

//void lines_1()
//{
// straight1();
// if(dis_x<-2)	right1();
// if(dis_x>2) 	lift1();
// if(yaw>1) 		right1();		 	   
// if(yaw<-1) 	lift1();
//}

//void angle_lift()
//{
//  in1=1;
//	in2=0;
//	in3=0;
//	in4=1;
//	speed1=45;  //r
//	speed2=20;  //l
//}
//void angle_right()
//{
//  in1=0;
//	in2=1;
//	in3=1;
//	in4=0;
//	speed1=20;  //r
//	speed2=45;  //l
//}

//void ling_2()
//{
//			straight1();
//		  if(yaw>-29) 			right1();		 	   
//		  if(yaw<-31) 			lift1();
//}

//void ling_3()
//{
//			straight1();
//		  if(yaw>-59) 			right1();		 	   
//		  if(yaw<-61) 			lift1();
//}

//void task5()
//{
//	static int h=0;
//	if(h==0)
//		{
//		if(dis_y<1000)
//			lines_1();
//		}	
//  if(dis_y>=1000) 
//	{
//		h=1;
//		stop();
//		delay_ms(100);
//	}
//		if(h==1)
//			{
//			   if(yaw>-30)
//	        angle_right();		
//			}
//		if(yaw<=-30) h=2;
//			if(h==2)
//				{
//					if(dis_x<350&&dis_y<1595)
//						ling_2();				 				
//				}
//				if(dis_x>=350&&dis_y>=1595) h=3;
//		if(h==3)
//			{
//					if(yaw>-60)
//	        angle_right();			
//			}
//			if(yaw>=-60) h=4;	
//   if(h==4)
//		 {
//			 if(dis_x<1000&&dis_y<2000)
//			 {
//			  ling_3();
//			 }
//			 if(dis_x>=1000&&dis_y>=2000)
//				 {
//					 h=5;
//					 while(1)stop();
//				 }
//		 
//		 }		 

//}
//void lift6()
//{
//  in1=1;
//	in2=0;
//	in3=1;
//	in4=0;
//	speed1=155;//85
//	speed2=135;//70
//}
//void right6()
//{
//  in1=1;
//	in2=0;
//	in3=1;
//	in4=0;
//	speed1=135;//72
//	speed2=140;//80
//}
//void straight6()
//{
//  in1=1;
//	in2=0;
//	in3=1;
//	in4=0;
//	speed1=100;
//	speed2=100;
//}
//void task6()
//{
//	 straight6();	
//   if(dis_y>=(6000*1.414))//&&dis_x<=20
//	 {
//		 led=!led; 
//		 while(1)stop();			 
//	 }	 
//	  if(yaw>2)	right6();
//	  else if(yaw<-1) 	lift6();//
//    else if(dis_x<-1) right6();//
//    else if(dis_x>1)	lift6();//
//    else straight1();// 
// }

// void lift7()
//{
//  in1=1;
//	in2=0;
//	in3=1;
//	in4=0;
//	speed1=155;//85
//	speed2=135;//70
//}
// void max_right7()
//{
//  in1=0;
//	in2=1;
//	in3=1;
//	in4=0;
//	speed1=20;//85
//	speed2=75;//70
//}
//void right7()
//{
//  in1=1;
//	in2=0;
//	in3=1;
//	in4=0;
//	speed1=135;//72
//	speed2=140;//80
//}
//void straight7()
//{
//  in1=1;
//	in2=0;
//	in3=1;
//	in4=0;
//	speed1=100;
//	speed2=100;
//}
//void task7_line1()
//{
//	static int m=0;
//	if(yaw>-45&&m==0)
//	{
//  while(yaw>-45)
//		{
//			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){}
//			display_task(yaw,0);
//			max_right7();
//				if(yaw<-45)
//					{
//						m=1;
//						break;
//					}
//		}
//  }else task7_line2();
//}
//void task7_line2()
//{
//	 straight7();	
//   if(dis_y>=6000&&dis_x>=6000)//&&dis_x<=20
//	 {
//		 led=!led; 
//		 while(1)stop();			 
//	 }	 
//	  if(yaw>-47)	right7();
//	  else if(yaw<-45) 	lift7();//
//    else if(dis_x<((distance/1.414)-1)) right7();//
//    else if(dis_x>((distance/1.414)+1))	lift7();//
//    else straight1();

//}
//void task7()
//{
//	task7_line1();
//}
