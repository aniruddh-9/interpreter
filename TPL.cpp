#include<bits/stdc++.h>
#include<string>
#include<map>
#include<vector>
#include<iterator>
using namespace std;

map<string,int> funcMap;
map<string,int> hmap;
map<string,int> valueMap;


vector<string> split(string, string);
string Succ(vector<string>);
string Pred(vector<string>);
string IsZero(vector<string>);
string IfThenElse(vector<string>);
vector<string> Evaluate(vector<string>);

vector<string> CompleteArgs(vector<string>);

vector<string> checkTypable(vector<string>);

void printVector(vector<string> inp){
    for(int i=0;i<inp.size();i++)
        cout<<inp[i]<<" ";
    cout<<endl;
}

vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

string Succ(vector<string> arg){
    if(arg[0].compare("0")==0){
        cout<<"E-succ rule applied"<<endl;
        cout<<"Succ 0->0"<<endl;
        return "0";
    }else{
        return Succ(Evaluate(arg));
    }
}

string Pred(vector<string> arg){
    if(arg[0].compare("0")==0){
        cout<<"E-Pred rule applied"<<endl;
        cout<<"Pred 0->0"<<endl;
        return "0";
    }else{
        return Pred(Evaluate(arg));
    }
}

string IsZero(vector<string> arg){
    if(arg[0].compare("0")==0){
        cout<<"E-IsZero rule applied"<<endl;
        cout<<"IsZero 0->0"<<endl;
        return "True";
    }else{
        return IsZero(Evaluate(arg));
    }
}

vector<string> CompleteArgs(vector<string> input){
    vector<string> result;
    map<string,int>::iterator it;
    
    int i=0;
    int ifCount=0;
    while(i<input.size()){
        it=valueMap.find(input[i]);
        if(it!=valueMap.end()){                 // a value;
            result.push_back(input[i]);
            break;
        }else{
            if(input[i].compare("If")==0){      // if it's a if
                ifCount++;
                result.push_back(input[i++]);
                while(ifCount!=0){
                    if(input[i].compare("If")==0)
                        ifCount++;
                    if(input[i].compare("Else")==0)
                        ifCount--;
                    result.push_back(input[i++]);
                }
                // baaki ka else
                vector<string> arg1(input.begin()+i,input.end());
                vector<string> remainder = CompleteArgs(arg1);
                result.insert(result.end(), remainder.begin(), remainder.end());
                break;
            }else if(input[i].compare("And")==0){  
                                            // if It's an AND
                result.push_back(input[i++]);
                vector<string> arg1 = CompleteArgs(vector<string>(input.begin()+i,input.end()));
                result.insert(result.end(), arg1.begin(), arg1.end());
                vector<string> arg2 = CompleteArgs(vector<string>(input.begin()+i+arg1.size(),input.end()));
                result.insert(result.end(), arg2.begin(), arg2.end());
                vector<string> arg3 = CompleteArgs(vector<string>(input.begin()+i+arg1.size()+arg2.size(),input.end()));
                result.insert(result.end(), arg3.begin(), arg3.end());
                                                        //And push kara Ab 3 Complete arguments push
                //printVector(result);
                break;
            }else{
                result.push_back(input[i++]);
            }
        }
    }
    return result;
}

string IfThenElse(vector<string> input){
    vector<string> ifPart;
    vector<string> thenPart;
    vector<string> elsePart;

    string eval;

    int ifCount=1;
    int i=0;

    ifPart=CompleteArgs(input);
    thenPart=CompleteArgs(vector<string>(input.begin()+ifPart.size()+1,input.end()));
    elsePart=CompleteArgs(vector<string>(input.begin()+ifPart.size()+thenPart.size()+2,input.end()));

    vector<string> TestCond=Evaluate(ifPart);
    cout<<"E-If rule applied"<<endl;
    vector<string> ExecPart;
    if(TestCond[0].compare("True")==0){
        ExecPart=Evaluate(thenPart);
        cout<<"E-IfTrue rule applied"<<endl;
    }else{
        ExecPart=Evaluate(elsePart);
        cout<<"E-IfFalse rule applied"<<endl;
    }

    eval = ExecPart[0];
    return eval;
}

string And(vector<string> input){
    string result;

    vector<string> arg1;
    vector<string> arg2;
    vector<string> arg3;
    
    arg1 = CompleteArgs(input);
    arg2 = CompleteArgs(vector<string>(input.begin()+arg1.size(),input.end()));
    arg3 = CompleteArgs(vector<string>(input.begin()+arg1.size()+arg2.size(),input.end()));

    vector<string> eval1 = Evaluate(arg1);
    cout<<"E-AND1 rule applied"<<endl;
    if(eval1[0].compare("True")==0){
        vector<string> eval2 = Evaluate(arg2);
        cout<<"E-AND2 rule applied"<<endl;
        if(eval2[0].compare("True")==0){
            vector<string> eval3 = Evaluate(arg3);
            cout<<"E-AND3 rule applied"<<endl;
            if(eval3[0].compare("True")==0){
                return "True";
            }
            else    
                return "False";
        }
        else
            return "False";
    }else   
        return "False";   
}

vector<string> Evaluate(vector<string> input){
    string op = input[0];
    string result;
    vector<string> r;
    vector<string> arg(input.begin()+1,input.end());

    if(op.compare("0")==0 || op.compare("True")==0 || op.compare("False")==0){
        r.push_back(op);
        return r;
    }

    switch (funcMap[op]){
        case 1:
                result=Succ(arg);
                break;
        case 2:
                result=Pred(arg);
                break;
        case 3:
                result=IsZero(arg);
                break;
        case 4:
                result=IfThenElse(arg);
                break;
        case 5:
                result=And(arg);
                break;
        default : exit(0);
    }

    r.push_back(result);
    return r;
}


string SuccTypable(vector<string> arg){
    if(arg[0].compare("0")==0)
        return "0";
    else if(arg[0].compare("True")==0 || arg[0].compare("False")==0 || arg[0].compare("Bool")==0){
        cout<<"Ill-Typed Term"<<endl;
        exit(0);
    }else{
        return SuccTypable(checkTypable(arg));
    }
}

string IsZeroTypable(vector<string> arg){
    if(arg[0].compare("0")==0)
        return "Bool";
    else if(arg[0].compare("True")==0 || arg[0].compare("False")==0 || arg[0].compare("Bool")==0){
        cout<<"Ill-Typed Term"<<endl;
        exit(0);
    }else{
        return IsZeroTypable(checkTypable(arg));
    }
}

string IfThenElseTypable(vector<string> input){
    vector<string> ifPart;
    vector<string> thenPart;
    vector<string> elsePart;
    string ifType;
    string thenType;
    string elseType;

    int i=0;

    ifPart=CompleteArgs(input);
    thenPart=CompleteArgs(vector<string>(input.begin()+ifPart.size()+1,input.end()));
    elsePart=CompleteArgs(vector<string>(input.begin()+ifPart.size()+thenPart.size()+2,input.end()));

    ifType = checkTypable(ifPart)[0];
    if(ifType.compare("0")==0){
        cout<<"Ill-Typed Term"<<endl;
        exit(0);
    }

    thenType = checkTypable(thenPart)[0];
    if(thenType.compare("True")==0 || thenType.compare("False")==0){
        thenType="Bool";
    }

    elseType = checkTypable(elsePart)[0];
    if(elseType.compare("True")==0 || elseType.compare("False")==0){
        elseType="Bool";
    }


    if(thenType.compare(elseType)!=0){    // handle if true then true else false
        cout<<"Ill-Typed Term"<<endl;
        exit(0);
    }

    return thenType;
}

string AndTypable(vector<string> input){
    vector<string> arg1;
    vector<string> arg2;
    vector<string> arg3;
    string arg1Type;
    string arg2Type;
    string arg3Type;
    
    arg1 = CompleteArgs(input);
    arg2 = CompleteArgs(vector<string>(input.begin()+arg1.size(),input.end()));
    arg3 = CompleteArgs(vector<string>(input.begin()+arg1.size()+arg2.size(),input.end()));

    arg1Type = checkTypable(arg1)[0];
    if(arg1Type.compare("0")!=0){
        arg2Type = checkTypable(arg2)[0];
        if(arg2Type.compare("0")!=0){
            arg3Type = checkTypable(arg3)[0];
            if(arg3Type.compare("0")!=0){
                return "Bool";
            }else{
                cout<<"Ill-Typed Term"<<endl;
                exit(0);
            }
        }else{
            cout<<"Ill-Typed Term"<<endl;
            exit(0);
        }
    }else{
        cout<<"Ill-Typed Term"<<endl;
        exit(0);
    }
}


vector<string> checkTypable(vector<string> input){
    string op = input[0];
    string type;                                       // 1 NAT ....0 Boolean
    vector<string> r;
    vector<string> arg(input.begin()+1,input.end());

    if(op.compare("True")==0 || op.compare("False")==0 || op.compare("0")==0){
        r.push_back(op);
        return r;
    }

    switch (funcMap[op]){
        case 1:
                type=SuccTypable(arg);
                break;
        case 2:
                type=SuccTypable(arg);
                break;
        case 3:
                type=IsZeroTypable(arg);
                break;
        case 4:
                type=IfThenElseTypable(arg);
                break;
        case 5:
                type=AndTypable(arg);
                break;
        default : exit(0);
    }
    r.push_back(type);
    return r;
}

int main(){
    vector<string>tokens;
    map<string,int>::iterator it;
    hmap["True"]=1;
    hmap["False"]=2;
    hmap["0"]=3;
    hmap["If"]=4;
    hmap["Then"]=5;
    hmap["Else"]=6;
    hmap["IsZero"]=7;
    hmap["Pred"]=8;
    hmap["Succ"]=9;
    hmap["And"]=10;

    funcMap["Succ"]=1;
    funcMap["Pred"]=2;
    funcMap["IsZero"]=3;
    funcMap["If"]=4;
    funcMap["And"]=5;

    valueMap["0"]=1;
    valueMap["True"]=2;
    valueMap["False"]=3;

    string str;
    cout<<"Enter the string of token\n";
    getline(cin,str);
    cout<<endl;
    tokens=split(str," ");

    // lexAnalyzer
    for(int i =0;i<tokens.size();i++){
        it=hmap.find(tokens[i]);
        if(it==hmap.end()){
            cout<<"Invalid Token"<<endl;;
            return 0;
        }
    }

    vector<string> type = checkTypable(tokens);

    vector<string> result = Evaluate(tokens);

    cout<<endl;
    cout<<"Result: "<<result[0]<<endl;
    
    return 0;
}