/* The  classes (All source codes in one file) (main.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include <fstream>
#include <vector>
using namespace std;
 

class Instruction_status_entry {
private:
public:
    string op;
    string target;
    string j;
    string k;
    int offset;
    // Constructor with default values for data members
    Instruction_status_entry(string op ,string target ,string j, string k, int offset = 0) {
      this -> op = op;
      this -> target = target;
      this -> j = j;
      this -> k = k;
      this -> offset = offset;
    }

};   // need to end the class declaration with a semi-colon

class LD_buffer_entry {
private:
public:
    bool busy = false;
    string address;
    int id = 1;
    int offset;
    int time; 
    string name;
    // Constructor with default values for data members
    LD_buffer_entry() {
      cout <<"constructor called"<<'\n';
    }
    LD_buffer_entry(bool busy , int id) {
      this -> busy = busy;
      this -> id = id;
      this -> name = "LOAD"+ to_string(id);
    }
    LD_buffer_entry(int address,int offset) {
      this -> address = address + offset;
    }

};   

class ST_buffer_entry {
private:
public:
    bool busy;
    string address;
    int id = 1;
    string name;
    // Constructor with default values for data members
    ST_buffer_entry() {}
    ST_buffer_entry(bool busy , int id) {
      this -> busy = busy;
      this -> id = id;
      this -> name = "LOAD"+ to_string(id);
    }
    ST_buffer_entry(int address,int offset) {
      this -> address = address + offset;
    }

}; 

class Reservation_station_entry_AS {
private:
public://Time Name Busy Op Vj Vk Qj Qk
    int Time;
    string Name;
    bool busy;
    int id;
    string Op;
    string Vj;
    string Vk;
    string Qj;
    string Qk;
    // Constructor with default values for data members
    Reservation_station_entry_AS() {}
    Reservation_station_entry_AS(bool busy , int id) {
      this -> busy = busy;
      this -> id = id;
    }  
};   

class Reservation_station_entry_MD {
private:
public://Time Name Busy Op Vj Vk Qj Qk
    int Time;
    string Name = "MULT";
    bool busy;
    int id;
    string Op;
    string Vj;
    string Vk;
    string Qj;
    string Qk;
    // Constructor with default values for data members
    Reservation_station_entry_MD() {}
    Reservation_station_entry_MD(bool busy , int id) {
      this -> busy = busy;
      this -> id = id;
    }  
};   

class Register_result_status_entry{
private:
public:
    string name;
    string FU;
    //bool waiting;
    Register_result_status_entry(){}
};

class Integer_register_entry{
private:
public:
    string name;
    string value;
    Integer_register_entry(){}
};

//declare functions
int find_empty_LD_buffer_entry(LD_buffer_entry LD_buffer[]);
int find_empty_ST_buffer_entry(ST_buffer_entry ST_buffer[]);
int find_empty_AS_reservation_station_entry(Reservation_station_entry_AS Reservation_station_AS[]);
int find_empty_MD_reservation_station_entry(Reservation_station_entry_MD Reservation_station_MD[]);
//global arrays
//static Reservation_station_entry_AS Reservation_station_AS[3]; // ADD/SUB reservation station
//static Reservation_station_entry_MD Reservation_station_MD[2]; // MUL/DIV reservation station
//static Integer_register_entry Integer_register[8]; 
//static Register_result_status_entry Register_result_status[15];// register result status
int main() {   
  ofstream myf ("example.txt");
  if (myf.is_open())
  {
    myf << "This is a line.\n";
    myf << "This is another line11212.\n";
    myf.close();
  }

   string line;
   vector <Instruction_status_entry> Instruction_status;   // instruction ststus
   LD_buffer_entry LD_buffer[3];                           // load buffer
   ST_buffer_entry ST_buffer[3];                           // store buffer
   Reservation_station_entry_AS Reservation_station_AS[3]; // ADD/SUB reservation station
   Reservation_station_entry_MD Reservation_station_MD[2]; // MUL/DIV reservation station
   Register_result_status_entry Register_result_status[15];// register result status
   Integer_register_entry Integer_register[8];             // integer registers
   /*instantiate reservation stations and buffers*/
   for(int i = 0; i < 3 ; i++){
     //LD_buffer[i](false,i);  
     LD_buffer[i].busy = false;
     LD_buffer[i].id = i;
     LD_buffer[i].name = "LOAD"+ to_string(i); 
   }
   for(int i = 0; i < 3 ; i++){
     //ST_buffer[i] = ST_buffer_entry(false,i);
     ST_buffer[i].busy = false;
     ST_buffer[i].id = i;
   }
   for(int i = 0; i < 3 ; i++){
     //Reservation_station_AS[i] = Reservation_station_entry_AS(false,i);
     Reservation_station_AS[i].busy = false;
     Reservation_station_AS[i].id = i;
     //Reservation_station_AS[i].Name = ;
   }
   for(int i = 0; i < 2 ; i++){
     //Reservation_station_MD[i] = Reservation_station_entry_MD(false,i);
     Reservation_station_MD[i].busy = false;
     Reservation_station_MD[i].id = i;
    // cout << "66666666666666"+Reservation_station_MD[i].Name<<'\n';
     Reservation_station_MD[i].Name = "MULT" + to_string(i);
   }
   for(int i = 0; i < 15 ; i++){
     Register_result_status[i] = Register_result_status_entry();
   }
   for(int i = 0; i < 8; i++){
     Integer_register[i] = Integer_register_entry();
   }
   int current_cycle = 0;
   int location;
   string op;
   string rest; 
   string target;
   int prent_location;
   int space_location;
   int offset;
   string j;
   string k;
   int reg_num;
   int LD_duffer_index;
   int MD_reservation_station_index;
   //ofstream myf ("example.txt");
   //myf.open("example.txt");
   //myf<<"wrot a line";
   //myf.close();
   ifstream myfile ("inst2.trace");

   if (myfile.is_open())
   {
     while ( getline (myfile,line) )
     {
       cout << line << '\n';
       current_cycle ++;
       location  = line.find(' ');
       op = line.substr(0,location);
       rest = line.substr(location+1);
       space_location = rest.find(' ');
       target = rest.substr(0,space_location);
       reg_num = stoi(target.substr(1));
       prent_location = rest.find('(');
       if(prent_location != -1){
        //cout << rest << '\n';
        //cout << space_location << '\n';
        //cout << prent_location << '\n';
        //cout << rest.substr(space_location+1,prent_location - space_location -1) <<'\n';
        offset = stoi(rest.substr(space_location+1,prent_location - space_location -1));
        k = rest.substr(prent_location+1,2);
        //cout << location << '\n';
        //cout << "cycle: " << current_cycle << '\n';
        //cout << "op: " <<op << '\n';
        //cout << "target: " << target << '\n';
        //cout << prent_location << '\n';
        //cout << "offset: " <<offset << '\n';
        //cout << "k: " << k << '\n';
        cout <<'\n';
        j = "";
        Instruction_status_entry is(op,target,j,k,offset);
        Instruction_status.push_back(is);
        //cout <<"test:" << is.op << '\n';
        if(is.op.compare("LW")==0){  //LD instruction 
          LD_duffer_index = find_empty_LD_buffer_entry(LD_buffer);
          LD_buffer[LD_duffer_index].address = k;
          LD_buffer[LD_duffer_index].offset = offset;
          LD_buffer[LD_duffer_index].time = current_cycle;
          LD_buffer[LD_duffer_index].busy = true;
          Register_result_status[reg_num].FU = LD_buffer[LD_duffer_index].name;
          //cout<<"here"<<'\n';
        }else{  //ST instruction

        }
        /*if (myfile1.is_open()){
          myfile1 << "This is a line.\n";
          myfile1.close();
        }*/

       }else{// ADD/SUB/LD/ST instructions
        j = rest.substr(space_location+1,2);
        k = rest.substr(space_location+4,2);
        offset = 0;
        Instruction_status_entry is(op,target,j,k,offset);
        Instruction_status.push_back(is);
        if(is.op.compare("MUL")==0){
          cout << op <<"   =============================="<<'\n';
          MD_reservation_station_index = find_empty_MD_reservation_station_entry(Reservation_station_MD);
          Reservation_station_MD[MD_reservation_station_index].Op = op;
          cout << "check reg  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<":::reg "+  Register_result_status[ stoi(j.substr(1))].FU+"::::"<< Register_result_status[ stoi(j.substr(1))].FU.substr(4)<<'\n';
        if(LD_buffer[ stoi(Register_result_status[ stoi(j.substr(1))].FU.substr(4)) ].busy){
          cout << "load is busy  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<'\n';
          Reservation_station_MD[MD_reservation_station_index].Qj = LD_buffer[stoi(j.substr(1))].name; //waiting for load
          Reservation_station_MD[MD_reservation_station_index].Vj = "x"; //rename register name
        }else{
          Reservation_station_MD[MD_reservation_station_index].Qj = "x"; //rename register name
          Reservation_station_MD[MD_reservation_station_index].Vj = "R("+j+")"; //rename register name
        }    
        Reservation_station_MD[MD_reservation_station_index].Vk = "R("+k+")"; //rename register name
        Reservation_station_MD[MD_reservation_station_index].busy = true; 
        cout << "+++++++++++++++++++++++++++++++++"+Reservation_station_MD[MD_reservation_station_index].Name <<'\n';
        Register_result_status[reg_num].FU = Reservation_station_MD[MD_reservation_station_index].Name ;// LD_buffer[LD_duffer_index].name;
        }
        /*MD_reservation_station_index = find_empty_MD_reservation_station_entry(Reservation_station_MD);
        Reservation_station_MD[MD_reservation_station_index].Op = op;
        cout << "check reg  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<":::reg "+  Register_result_status[ stoi(j.substr(1))].FU+"::::"<< Register_result_status[ stoi(j.substr(1))].FU.substr(4)<<'\n';
        if(LD_buffer[ stoi(Register_result_status[ stoi(j.substr(1))].FU.substr(4)) ].busy){
          cout << "load is busy  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<'\n';
          Reservation_station_MD[MD_reservation_station_index].Qj = LD_buffer[stoi(j.substr(1))].name; //waiting for load
          Reservation_station_MD[MD_reservation_station_index].Vj = "x"; //rename register name
        }else{
          Reservation_station_MD[MD_reservation_station_index].Qj = "x"; //rename register name
          Reservation_station_MD[MD_reservation_station_index].Vj = "R("+j+")"; //rename register name
        }    
        Reservation_station_MD[MD_reservation_station_index].Vk = "R("+k+")"; //rename register name
        Reservation_station_MD[MD_reservation_station_index].busy = true; */
        //Register_result_status[reg_num].FU 

        //cout << "cycle: " << current_cycle << '\n';
        //cout << "op: " <<op << '\n';
        //cout << "target: " << target << '\n';
        //cout << prent_location << '\n';
        //cout << "j: " <<j << '\n';
        //cout << "k: " << k << '\n';
        cout <<'\n';
        //Instruction_status_entry is(op,target,j,k,offset);
        //Instruction_status.push_back(is);
        //cout <<"test:" << is.op << '\n';

       }
       /* cycle snapshot section */
       cout << "---------------" << '\n';
       cout << "Cycle " << current_cycle << '\n';
       for(int i = 0;i < 3 ;i++){
         if(LD_buffer[i].busy == true){
           cout << "LOAD[" << LD_buffer[i].id << "]: "<< LD_buffer[i].address << "(" << LD_buffer[i].offset << ")(time:" <<LD_buffer[i].time <<")\n"; 
         }
       }
       for(int  i = 0; i < 2 ; i++){
          if(Reservation_station_MD[i].busy == true){
            cout << "MULT[" << Reservation_station_MD[i].id << "]: MULTD "<< Reservation_station_MD[i].Vj<< " "<<Reservation_station_MD[i].Vk<<" " <<Reservation_station_MD[i].Qj<<" "<<Reservation_station_MD[i].Qk <<'\n';
          }
       }
       cout << "Waiting register:";
       for(int i = 0;i < 15 ; i++){
        
        cout <<"F"<< i <<"("<< Register_result_status[i].FU<< ") ";
       }
       cout << '\n';
     }
     myfile.close();
     //myfile1.close();
   }

   else cout << "Unable to open file"; 
   /*Testing section*/ 
    cout << "Instruction_status contains:";
    for(int i = 0; i < Instruction_status.size();i ++){
      cout << Instruction_status[i].op << '\n';
    }
    cout << LD_buffer[0].busy <<'\n';
    int id = find_empty_LD_buffer_entry(LD_buffer);
    cout << id <<'\n';
   return 0;
}

int find_empty_LD_buffer_entry(LD_buffer_entry LD_buffer[]){
  int id;
  for(int i = 0; i < 3 ; i++){
    if(LD_buffer[i].busy == false){
      id = LD_buffer[i].id;
      break;
    }  
  }
  return id;
}

int find_empty_ST_buffer_entry(ST_buffer_entry ST_buffer[]){
  int id;
  for(int i = 0; i < 3 ; i++){
    if(ST_buffer[i].busy == false){
      id = ST_buffer[i].id;
      break;
    }  
  }
  return id;
}

int find_empty_AS_reservation_station_entry(Reservation_station_entry_AS Reservation_station_AS[]){
  int id;
  for(int i = 0; i < 3 ; i++){
    if(Reservation_station_AS[i].busy == false){
      id = Reservation_station_AS[i].id;
      break;
    }  
  }
  return id;
}

int find_empty_MD_reservation_station_entry(Reservation_station_entry_MD Reservation_station_MD[]){
  int id;
  for(int i = 0; i < 3 ; i++){
    if(Reservation_station_MD[i].busy == false){
      id = Reservation_station_MD[i].id;
      break;
    }  
  }
  return id;
}

/*void find_corespnding_buffer(int id,Register_result_status_entry Register_result_status[]){
  Register_result_status[id].FU = LD_buffer[]
}*/
/*
ofstream myfile ("example.txt");
  if (myfile.is_open())
  {
    myfile << "This is a line.\n";
    myfile << "This is another line.\n";
    myfile.close();
  }

*/