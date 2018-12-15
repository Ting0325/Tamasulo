/* The  classes (All source codes in one file) (main.cpp) */
#include <iostream>    // using IO functions
#include <string>      // using string
#include <fstream>
#include <vector>
using namespace std;
 

class Instruction_status_entry {
private:
public:
    string instruction;
    int Execution_complete;
    string Write_result;
    string op;
    string target;
    string j;
    string k;
    int offset;
    int issue;
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
    int issue;
    string result;
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
    int j;
    int k;
    int target;
    int start_cycle = -1;
    int issue;
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
    int j;
    int k;
    int target;
    int start_cycle = -1;
    int issue;
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
    bool waiting = false;
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
void check_LD_finish(int current_cycle,LD_buffer_entry LD_buffer[],vector <Instruction_status_entry> Instruction_status);
//global arrays
//static Reservation_station_entry_AS Reservation_station_AS[3]; // ADD/SUB reservation station
//static Reservation_station_entry_MD Reservation_station_MD[2]; // MUL/DIV reservation station
//static Integer_register_entry Integer_register[8]; 
//static Register_result_status_entry Register_result_status[15];// register result status
static int next_issue = 0;
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
     Reservation_station_AS[i].Name = "ADD"+ to_string(i);
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
     Register_result_status[i].FU = "R(F"+ to_string(i)+")";
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
   int MD_reservation_station_index,AS_reservation_station_index;
   //ofstream myf ("example.txt");
   //myf.open("example.txt");
   //myf<<"wrot a line";
   //myf.close();
   ifstream myfile ("inst2.trace");

   if (myfile.is_open())
   {
     while ( getline (myfile,line) )
     {
       next_issue++;
       cout << line <<" issue: "<< next_issue<< '\n';
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
        is.issue = next_issue;
        is.instruction = line;
        Instruction_status.push_back(is);
        cout << "Instruction_status added "<<Instruction_status.size()-1<<" " <<Instruction_status[Instruction_status.size()-1].instruction<<" issue" << is.issue<<'\n';
        //cout <<"test:" << is.op << '\n';
        if(is.op.compare("LW")==0){  //LD instruction 
          LD_duffer_index = find_empty_LD_buffer_entry(LD_buffer);
          LD_buffer[LD_duffer_index].address = k + "+" +to_string(offset) ;
          LD_buffer[LD_duffer_index].offset = offset;
          LD_buffer[LD_duffer_index].time = current_cycle;
          LD_buffer[LD_duffer_index].issue = next_issue;
          LD_buffer[LD_duffer_index].busy = true;
          cout<<"set ld_buffer busy buffer:"<< LD_duffer_index<<'\n';
          Register_result_status[reg_num].FU = LD_buffer[LD_duffer_index].name;
          Register_result_status[reg_num].waiting = LD_buffer[LD_duffer_index].busy;
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
        is.issue = next_issue;
        is.instruction = line;
        Instruction_status.push_back(is);
        if(is.op.compare("MUL")==0){
          cout << op <<"   =============================="<<'\n';
          MD_reservation_station_index = find_empty_MD_reservation_station_entry(Reservation_station_MD);
          Reservation_station_MD[MD_reservation_station_index].issue = is.issue;
          Reservation_station_MD[MD_reservation_station_index].Op = op;
          Reservation_station_MD[MD_reservation_station_index].j = stoi(j.substr(1));
          Reservation_station_MD[MD_reservation_station_index].k = stoi(k.substr(1));
          cout << "check reg  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<":::reg "+  Register_result_status[ stoi(j.substr(1))].FU+"::::"<< Register_result_status[ stoi(j.substr(1))].FU.substr(4)<<'\n';
          if(LD_buffer[ stoi(Register_result_status[ stoi(j.substr(1))].FU.substr(4)) ].busy){
            cout << "load is busy  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<'\n';
            Reservation_station_MD[MD_reservation_station_index].Qj = Register_result_status[ stoi(j.substr(1))].FU;//LD_buffer[stoi(j.substr(1))].name; //waiting for load
            Reservation_station_MD[MD_reservation_station_index].Vj = "x"; //rename register name
          }else{
            Reservation_station_MD[MD_reservation_station_index].Qj = "x"; //rename register name
            Reservation_station_MD[MD_reservation_station_index].Vj = "R("+j+")"; //rename register name
          }    
          Reservation_station_MD[MD_reservation_station_index].Vk = "R("+k+")"; //rename register name
          Reservation_station_MD[MD_reservation_station_index].Time = 10;
          Reservation_station_MD[MD_reservation_station_index].busy = true;
          Reservation_station_MD[MD_reservation_station_index].target = reg_num;
          cout << "+++++++++++++++++++++++++++++++++"+Reservation_station_MD[MD_reservation_station_index].Name <<'\n';
          Register_result_status[reg_num].FU = Reservation_station_MD[MD_reservation_station_index].Name ;// LD_buffer[LD_duffer_index].name;

        }else if(is.op.compare("SUB")==0){
          cout << op <<"   =============================="<<'\n';
          AS_reservation_station_index = find_empty_AS_reservation_station_entry(Reservation_station_AS);
          Reservation_station_AS[AS_reservation_station_index].issue = is.issue;
          Reservation_station_AS[AS_reservation_station_index].Op = op;
          cout << "j : "<< j  <<"stoi(j.substr(1)) : " <<stoi(j.substr(1))<<"look over here"<<'\n';
          Reservation_station_AS[AS_reservation_station_index].j = stoi(j.substr(1));
          Reservation_station_AS[AS_reservation_station_index].k = stoi(k.substr(1));
          cout << "check reg  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<":::reg "+  Register_result_status[ stoi(j.substr(1))].FU+"::::"<< Register_result_status[ stoi(j.substr(1))].FU.substr(4)<<'\n';
          if(Register_result_status[ stoi(j.substr(1))].waiting){
            cout << "reg is waiting  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Qj = Register_result_status[ stoi(j.substr(1))].FU;//LD_buffer[stoi(j.substr(1))].name; //waiting for load
            Reservation_station_AS[AS_reservation_station_index].Vj = "x"; //rename register name
          }else{
            Reservation_station_AS[AS_reservation_station_index].Qj = "x"; //rename register name
            Reservation_station_AS[AS_reservation_station_index].Vj =  Register_result_status[ stoi(j.substr(1))].FU; 
          }    
          cout << k+"::::"<< k.substr(1)+" :::: "  << stoi(k.substr(1))<<'\n';
          if(Register_result_status[ stoi(k.substr(1))].waiting){
            cout << "reg is waiting  :::::"<< k+"::::"<< k.substr(1)+" :::: "  << stoi(k.substr(1))<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Qk = Register_result_status[ stoi(k.substr(1))].FU;//LD_buffer[stoi(k.substr(1))].name; //waiting for load
            Reservation_station_AS[AS_reservation_station_index].Vk = "x"; //rename register name
          }else{
            Reservation_station_AS[AS_reservation_station_index].Qk = "x"; //rename register name
            //cout<< "u left here"<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Vk =  Register_result_status[ stoi(k.substr(1))].FU; 
          }
         // Reservation_station_AS[AS_reservation_station_index].Vk = "R("+k+")"; //rename register name
          Reservation_station_AS[AS_reservation_station_index].Time = 1;
          Reservation_station_AS[AS_reservation_station_index].busy = true; 
          Reservation_station_AS[AS_reservation_station_index].target = reg_num;
          cout << "+++++++++++++++++++++++++++++++++"+Reservation_station_AS[AS_reservation_station_index].Name <<'\n';
          Register_result_status[reg_num].FU = Reservation_station_AS[AS_reservation_station_index].Name ;// LD_buffer[LD_duffer_index].name;
        }else if(is.op.compare("DIV")==0){
          cout << op <<"   =============================="<<'\n';
          MD_reservation_station_index = find_empty_MD_reservation_station_entry(Reservation_station_MD);
          Reservation_station_MD[MD_reservation_station_index].issue = is.issue;
          Reservation_station_MD[MD_reservation_station_index].Op = op;
          Reservation_station_MD[MD_reservation_station_index].j = stoi(j.substr(1));
          Reservation_station_MD[MD_reservation_station_index].k = stoi(k.substr(1));
          cout << "check reg  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<":::reg "+  Register_result_status[ stoi(j.substr(1))].FU+"::::"<< Register_result_status[ stoi(j.substr(1))].FU.substr(4)<<'\n';
          if(Register_result_status[ stoi(j.substr(1))].waiting){
            cout << "load is busy  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<'\n';
            Reservation_station_MD[MD_reservation_station_index].Qj = Register_result_status[ stoi(j.substr(1))].FU;;//LD_buffer[stoi(j.substr(1))].name; //waiting for load
            Reservation_station_MD[MD_reservation_station_index].Vj = "x"; //rename register name
          }else{
            Reservation_station_MD[MD_reservation_station_index].Qj = "x"; //rename register name
            Reservation_station_MD[MD_reservation_station_index].Vj = "R("+j+")"; //rename register name
          } 
          cout << k+"::::"<< k.substr(1)+" :::: "  << stoi(k.substr(1))<<'\n';
          if(Register_result_status[ stoi(k.substr(1))].waiting){
            cout << "reg is waiting  :::::"<< k+"::::"<< k.substr(1)+" :::: "  << stoi(k.substr(1))<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Qk = Register_result_status[ stoi(k.substr(1))].FU; //LD_buffer[stoi(k.substr(1))].name; //waiting for load
            Reservation_station_AS[AS_reservation_station_index].Vk = "x"; //rename register name
          }else{
            Reservation_station_AS[AS_reservation_station_index].Qk = "x"; //rename register name
            Reservation_station_AS[AS_reservation_station_index].Vk =  Register_result_status[ stoi(k.substr(1))].FU; 
          }   
          //Reservation_station_MD[MD_reservation_station_index].Vk = "R("+k+")"; //rename register name
          Reservation_station_MD[MD_reservation_station_index].Time = 0;
          Reservation_station_MD[MD_reservation_station_index].busy = true; 
          cout << "+++++++++++++++++++++++++++++++++"+Reservation_station_MD[MD_reservation_station_index].Name <<'\n';
          Register_result_status[reg_num].FU = Reservation_station_MD[MD_reservation_station_index].Name ;// LD_buffer[LD_duffer_index].name;
        }else if(is.op.compare("ADD")==0){
          cout << op <<"   =============================="<<'\n';
          AS_reservation_station_index = find_empty_AS_reservation_station_entry(Reservation_station_AS);
          Reservation_station_AS[AS_reservation_station_index].issue = is.issue;
          Reservation_station_AS[AS_reservation_station_index].Op = op;
          cout << "j : "<< j  <<"stoi(j.substr(1)) : " <<stoi(j.substr(1))<<"look over here"<<'\n';
          Reservation_station_AS[AS_reservation_station_index].j = stoi(j.substr(1));
          Reservation_station_AS[AS_reservation_station_index].k = stoi(k.substr(1));
          cout << "check reg  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<":::reg "+  Register_result_status[ stoi(j.substr(1))].FU+"::::"<< Register_result_status[ stoi(j.substr(1))].FU.substr(4)<<'\n';
          if(Register_result_status[ stoi(j.substr(1))].waiting){
            cout << "reg is waiting  :::::"<< j+"::::"<< j.substr(1)+" :::: "  << stoi(j.substr(1))<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Qj = Register_result_status[ stoi(j.substr(1))].FU;//LD_buffer[stoi(j.substr(1))].name; //waiting for load
            Reservation_station_AS[AS_reservation_station_index].Vj = "x"; //rename register name
          }else{
            Reservation_station_AS[AS_reservation_station_index].Qj = "x"; //rename register name
            Reservation_station_AS[AS_reservation_station_index].Vj =  Register_result_status[ stoi(j.substr(1))].FU; 
          }    
          cout << k+"::::"<< k.substr(1)+" :::: "  << stoi(k.substr(1))<<'\n';
          if(Register_result_status[ stoi(k.substr(1))].waiting){
            cout << "reg is waiting  :::::"<< k+"::::"<< k.substr(1)+" :::: "  << stoi(k.substr(1))<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Qk = Register_result_status[ stoi(k.substr(1))].FU;//LD_buffer[stoi(k.substr(1))].name; //waiting for load
            Reservation_station_AS[AS_reservation_station_index].Vk = "x"; //rename register name
          }else{
            Reservation_station_AS[AS_reservation_station_index].Qk = "x"; //rename register name
            //cout<< "u left here"<<'\n';
            Reservation_station_AS[AS_reservation_station_index].Vk =  Register_result_status[ stoi(k.substr(1))].FU; 
          }
         // Reservation_station_AS[AS_reservation_station_index].Vk = "R("+k+")"; //rename register name
          Reservation_station_AS[AS_reservation_station_index].Time = 0;
          Reservation_station_AS[AS_reservation_station_index].busy = true; 
          Reservation_station_AS[AS_reservation_station_index].target = reg_num;
          cout << "+++++++++++++++++++++++++++++++++"+Reservation_station_AS[AS_reservation_station_index].Name <<'\n';
          Register_result_status[reg_num].FU = Reservation_station_AS[AS_reservation_station_index].Name ;// LD_buffer[LD_duffer_index].name;
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
       /*update reservation station contents*/
       
       for(int i = 0 ; i < 3; i ++){//update j for AS
          cout << "Reservation_station_AS[i].j : "<<Reservation_station_AS[i].j<<'\n';
          if(Reservation_station_AS[i].busy == true){
            if(Register_result_status[ Reservation_station_AS[i].j].waiting){
              Reservation_station_AS[i].Vj = "x";
              Reservation_station_AS[i].Qj = Register_result_status[ Reservation_station_AS[i].j].FU;
            }else{
              if(current_cycle<6){
                Reservation_station_AS[i].Vj = Register_result_status[ Reservation_station_AS[i].j].FU;
                Reservation_station_AS[i].Qj = "x";
              }
              
            }
          }
         
       }
       for(int i = 0 ; i < 3; i ++){//update k for AS
        if(Reservation_station_AS[i].busy == true){
          if(Register_result_status[ Reservation_station_AS[i].k].waiting){
            Reservation_station_AS[i].Vk = "x";
            Reservation_station_AS[i].Qk = Register_result_status[ Reservation_station_AS[i].k].FU;
          }else{
            if(current_cycle<6){
              Reservation_station_AS[i].Vk = Register_result_status[ Reservation_station_AS[i].k].FU;
              Reservation_station_AS[i].Qk = "x";
            }
            
         }
        }

       }
       for(int i = 0 ; i < 2; i ++){//update j for MD
        if(Reservation_station_MD[i].busy == true){
          if(Register_result_status[ Reservation_station_MD[i].j].waiting){
            Reservation_station_MD[i].Vj = "x";
            Reservation_station_MD[i].Qj = Register_result_status[ Reservation_station_MD[i].j].FU;
          }else {
            if(current_cycle<6){
              Reservation_station_MD[i].Vj = Register_result_status[ Reservation_station_MD[i].j].FU;
              Reservation_station_MD[i].Qj = "x";
            }

          } 
        }
       }
       for(int i = 0 ; i < 2; i ++){//update k for MD
        if(Reservation_station_MD[i].busy == true){
         if(Register_result_status[ Reservation_station_MD[i].k].waiting ){//&& Reservation_station_MD[i].Vk.substr(1,0).compare("R")!=0){
            Reservation_station_MD[i].Vk = "x";
            Reservation_station_MD[i].Qk = Register_result_status[ Reservation_station_MD[i].k].FU;
         }else {//if(Reservation_station_MD[i].Vk.substr(1,0).compare("R")!=0){
           if(current_cycle< 6){
            Reservation_station_MD[i].Vk = Register_result_status[ Reservation_station_MD[i].k].FU;
            Reservation_station_MD[i].Qk = "x";
           }

         }
        }
       }

       //check if we can start execution of a certain instruction
       for(int i = 0 ; i < 3;i++){ //AS
          if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].Qj.compare("x") == 0 && Reservation_station_AS[i].Qk.compare("x")== 0 && Reservation_station_AS[i].start_cycle == -1){//is busy and waiting for nothing and hasnt started yet
            Reservation_station_AS[i].start_cycle = current_cycle;
            if(Reservation_station_AS[i].Op.compare("ADD") == 0){
                Reservation_station_AS[i].Time = 2;
            }else if(Reservation_station_AS[i].Op.compare("SUB") == 0){
                Reservation_station_AS[i].Time = 2;
            }
          }else if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].start_cycle!=-1){//decrement time
            Reservation_station_AS[i].Time --;
          }
       }
       for(int i = 0 ;i < 2;i++){ //MD
          if(Reservation_station_MD[i].busy == true && Reservation_station_MD[i].Qj.compare("x") == 0 && Reservation_station_MD[i].Qk.compare("x")== 0 && Reservation_station_MD[i].start_cycle == -1){//is busy and waiting for nothing and hasnt started yet
            Reservation_station_MD[i].start_cycle = current_cycle;
            if(Reservation_station_MD[i].Op.compare("MUL") == 0){
                Reservation_station_MD[i].Time = 10;
            }else if(Reservation_station_MD[i].Op.compare("DIV") == 0){
                Reservation_station_MD[i].Time = 40;
            }
          }else if(Reservation_station_MD[i].busy == true && Reservation_station_MD[i].start_cycle!=-1){//decrement time
            Reservation_station_MD[i].Time --;
          }
       }
       /* cycle snapshot section */
       cout << "---------------" << '\n';
       cout << "Cycle " << current_cycle << '\n';
       for(int i = 0;i < 3 ;i++){
         if(LD_buffer[i].busy == true){
           cout << "LOAD[" << LD_buffer[i].id << "]: "<< LD_buffer[i].address << "(" << LD_buffer[i].offset << ")(time:" <<LD_buffer[i].time <<")\n"; 
         }
       }
      for(int  i = 0; i < 3 ; i++){
          if(Reservation_station_AS[i].busy == true){
            cout << "ADD[" << Reservation_station_AS[i].id << "]: SUBD "<< Reservation_station_AS[i].Vj<< " "<<Reservation_station_AS[i].Vk<<" " <<Reservation_station_AS[i].Qj<<" "<<Reservation_station_AS[i].Qk <<" j:"<<Reservation_station_AS[i].j<<" k:"<<Reservation_station_AS[i].k<< "Time: "<< Reservation_station_AS[i].Time <<'\n';
          }
       }
       cout<<"**************************"<<"Register_result_status[2].FU: "<<Register_result_status[2].FU<<'\n';
      for(int  i = 0; i < 2 ; i++){
          if(Reservation_station_MD[i].busy == true){
            cout << "MULT[" << Reservation_station_MD[i].id << "]: "<< Reservation_station_MD[i].Op<< "D " << Reservation_station_MD[i].Vj<< " "<<Reservation_station_MD[i].Vk<<" " <<Reservation_station_MD[i].Qj<<" "<<Reservation_station_MD[i].Qk<< "Time: "<<Reservation_station_MD[i].Time <<'\n';
          }
       }
       cout << "Waiting register:";
       for(int i = 0;i < 15 ; i++){
        
        cout <<"F"<< i <<"("<< Register_result_status[i].FU<< ") ";
       }
       cout<< '\n';
       //check_LD_finish( current_cycle,LD_buffer , Instruction_status);
       for(int i = 0 ;i < 3;i++){
        cout<< i<<"current_cycle - LD_buffer[i].time = " << current_cycle - LD_buffer[i].time <<" LD["<<i<<"] busy? "<< LD_buffer[i].busy<<'\n';
        if(current_cycle - LD_buffer[i].time == 2 && LD_buffer[i].busy == true){ //check latency;
          cout<<"execution complete !!!!!     issue:"<< LD_buffer[i].issue<<" instruction " <<Instruction_status[LD_buffer[i].issue-1].instruction<<'\n';
          LD_buffer[i].result = "M("+ LD_buffer[i].address+")";
          LD_buffer[i].busy = false;                //nolonger busy
          Instruction_status[LD_buffer[i].issue-1].Execution_complete = current_cycle;    //update instruction status
          cout << Instruction_status[LD_buffer[i].issue-1].target<<"TTTTTTTTTTTTTTTTTTTTTTTTTTTTT"<<'\n';
      
          Register_result_status[stoi(Instruction_status[LD_buffer[i].issue-1].target.substr(1))].FU = LD_buffer[i].result;
          Register_result_status[stoi(Instruction_status[LD_buffer[i].issue-1].target.substr(1))].waiting = LD_buffer[i].busy;
          //LD_buffer.time = 0;//reset time
        }
       }
       //check reservation stations
       for(int i = 0 ; i< 3 ; i++){//AS
         if(Reservation_station_AS[i].busy){
           Register_result_status[Reservation_station_AS[i].target].waiting = Reservation_station_AS[i].busy;
         }
       }
       for(int i = 0 ; i< 2 ; i++){//MD
         if(Reservation_station_MD[i].busy){
           Register_result_status[Reservation_station_MD[i].target].waiting = Reservation_station_MD[i].busy;
         }
       }
       cout << '\n';
     }
     myfile.close();
     //myfile1.close();
   } else cout << "Unable to open file"; 
   /*stage 2 starts here*/
   for(current_cycle = 7;current_cycle<17; current_cycle++){
    //check if we can start execution of a certain instruction and update Time of each reservation station
       for(int i = 0 ; i < 3;i++){ //AS
          if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].Qj.compare("x") == 0 && Reservation_station_AS[i].Qk.compare("x")== 0 && Reservation_station_AS[i].start_cycle == -1){//is busy and waiting for nothing and hasnt started yet
            Reservation_station_AS[i].start_cycle = current_cycle;
            if(Reservation_station_AS[i].Op.compare("ADD") == 0){
                Reservation_station_AS[i].Time = 2;
            }else if(Reservation_station_AS[i].Op.compare("SUB") == 0){
                Reservation_station_AS[i].Time = 2;
            }
          }else if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].start_cycle!=-1){//during execution decrement time
            Reservation_station_AS[i].Time --;
          }//else 
          if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].Time == 0 && Reservation_station_AS[i].start_cycle != -1){//finished execution
            Instruction_status[ Reservation_station_AS[i].issue-1].Execution_complete = current_cycle;
            Reservation_station_AS[i].busy = false;//no longer busy
            Register_result_status[Reservation_station_AS[i].target].waiting = false;//update register result status
            Register_result_status[Reservation_station_AS[i].target].FU = "M()-M()";
            cout << "FFFFFFFFFFFFFFFF " <<"issue : "<<Reservation_station_AS[i].issue<<" finished at cycle"<< current_cycle << '\n';
          }
       }
       for(int i = 0 ;i < 2;i++){ //MD
          if(Reservation_station_MD[i].busy == true && Reservation_station_MD[i].Qj.compare("x") == 0 && Reservation_station_MD[i].Qk.compare("x")== 0 && Reservation_station_MD[i].start_cycle == -1){//is busy and waiting for nothing and hasnt started yet
            Reservation_station_MD[i].start_cycle = current_cycle;
            if(Reservation_station_MD[i].Op.compare("MUL") == 0){
                Reservation_station_MD[i].Time = 10;
            }else if(Reservation_station_MD[i].Op.compare("DIV") == 0){
                Reservation_station_MD[i].Time = 40;
            }
          }else if(Reservation_station_MD[i].busy == true && Reservation_station_MD[i].start_cycle!=-1){//decrement time
            Reservation_station_MD[i].Time --;
          }//else 
          if(Reservation_station_MD[i].busy == true && Reservation_station_MD[i].Time == 0 && Reservation_station_MD[i].start_cycle!=-1){//finished execution
            Instruction_status[ Reservation_station_MD[i].issue-1].Execution_complete = current_cycle;
            Reservation_station_MD[i].busy = false;//no longer busy
            cout <<"FFFFFFFFFFFFFFFF " <<"issue : "<<Reservation_station_AS[i].issue<<" finished at cycle"<< current_cycle << '\n';
          }
       }
      //don't forget to update reservation stations !!!(may also start some executions)
      for(int i = 0; i < 3; i++){
        //those that are busy but hasnt started are waiting 
        if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].start_cycle == -1){
          if(Register_result_status[Reservation_station_AS[i].j].waiting == false){//no longer waiting for sth
            Reservation_station_AS[i].Vj = Register_result_status[Reservation_station_AS[i].j].FU;
            Reservation_station_AS[i].Qj = "x";
          }
        }
        //check if some execution has statred
        /*if(Reservation_station_AS[i].busy == true && Reservation_station_AS[i].Qj.compare("x") == 0 && Reservation_station_AS[i].Qk.compare("x")== 0 && Reservation_station_AS[i].start_cycle == -1){//is busy and waiting for nothing and hasnt started yet
            Reservation_station_AS[i].start_cycle = current_cycle;
            if(Reservation_station_AS[i].Op.compare("ADD") == 0){
                Reservation_station_AS[i].Time = 2;
            }else if(Reservation_station_AS[i].Op.compare("SUB") == 0){
                Reservation_station_AS[i].Time = 2;
            }
        }*/
      } 
      /* cycle snapshot section */
       cout << "---------------" << '\n';
       cout << "Cycle " << current_cycle << '\n';
       for(int i = 0;i < 3 ;i++){
         if(LD_buffer[i].busy == true){
           cout << "LOAD[" << LD_buffer[i].id << "]: "<< LD_buffer[i].address << "(" << LD_buffer[i].offset << ")(time:" <<LD_buffer[i].time <<")\n"; 
         }
       }
      for(int  i = 0; i < 3 ; i++){
          if(Reservation_station_AS[i].busy == true){
            cout << "ADD[" << Reservation_station_AS[i].id << "]: SUBD "<< Reservation_station_AS[i].Vj<< " "<<Reservation_station_AS[i].Vk<<" " <<Reservation_station_AS[i].Qj<<" "<<Reservation_station_AS[i].Qk <<" j:"<<Reservation_station_AS[i].j<<" k:"<<Reservation_station_AS[i].k<< " Time: "<< Reservation_station_AS[i].Time <<'\n';
          }
       }
       cout<<"**************************"<<"Register_result_status[2].FU: "<<Register_result_status[2].FU<<'\n';
      for(int  i = 0; i < 2 ; i++){
          if(Reservation_station_MD[i].busy == true){
            cout << "MULT[" << Reservation_station_MD[i].id << "]: "<< Reservation_station_MD[i].Op<< "D " << Reservation_station_MD[i].Vj<< " "<<Reservation_station_MD[i].Vk<<" " <<Reservation_station_MD[i].Qj<<" "<<Reservation_station_MD[i].Qk<< " Time: "<<Reservation_station_MD[i].Time <<'\n';
          }
       }
       cout << "Waiting register:";
       for(int i = 0;i < 15 ; i++){
        
        cout <<"F"<< i <<"("<< Register_result_status[i].FU<< ") ";
       }
       cout<< '\n';
   }
    /*stage 3 starts here*/
   /*Testing section*/ /*final dump*/
    cout << '\n';
    cout <<"--------------------------------------------------"<<'\n';   
    cout << "Instruction_status contains:"<<'\n';
    cout << "Instruction"<<"   Is "<<"Ex "<<"Wb "<<"Cm "<<'\n';
    for(int i = 0; i < Instruction_status.size();i ++){
      cout<<Instruction_status[i].instruction<< "   "<<Instruction_status[i].issue<<" "<<Instruction_status[i].Execution_complete<< " wb " << Instruction_status[i].op << '\n';
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

void check_LD_finish(int current_cycle,LD_buffer_entry LD_buffer[],vector <Instruction_status_entry> Instruction_status){
  for(int i = 0 ;i < 3;i++){
    cout<< i<<"current_cycle - LD_buffer[i].time = " << current_cycle - LD_buffer[i].time <<'\n';
    if(current_cycle - LD_buffer[i].time == 2){ //check latency;
    cout<<"execution complete !!!!!     issue:"<< LD_buffer[i].issue<<'\n';
      LD_buffer[i].busy = false;                //nolonger busy
      Instruction_status[LD_buffer[i].issue-1].Execution_complete = current_cycle;    //update instruction status
      //LD_buffer.time = 0;//reset time
    }
  }
}

void print_cycle_table_for_instructions(vector <Instruction_status_entry> Instruction_status){
  for(int i = 0; i < Instruction_status.size();i++){

  }
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