#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool saftey(vector<int> available, vector<vector<int>> need, vector<vector<int>> allocation, int resources, int processes, int process_requested, int func_flag, int request_flag);
void sequence_print(vector <int> sequence, bool safe, int processes, int func_flag, int process_requested, int request_flag, int resources);
void request(vector<int> available, vector<vector<int>> need, vector<vector<int>> allocation, int resources, int processes);
void matrices_input(vector<int> &available, vector<vector<int>> &need, vector<vector<int>> &allocation, vector<vector<int>> &Max_resources, int resources, int processes);

int main()
{
    int resources;
    int processes;
    cout << "Enter number of processes:" << endl;
    cin >> processes;
    cout << "Enter number of resources:" << endl;
    cin >> resources;

    vector <vector <int>> Max_resources(processes, vector<int>(resources));
    vector <vector <int>> allocation(processes, vector<int>(resources));
    vector <vector <int>> need(processes, vector<int>(resources));
    vector <int> available(resources);

// function to get user inputs.
    matrices_input(available, need, allocation, Max_resources, resources, processes);

// saftey check.
    string saftey_inquiry;
    cout << "Is safe state needed? [Yes/No]"<<endl;
    cin >> saftey_inquiry;
    bool safe =false;
    transform(saftey_inquiry.begin(), saftey_inquiry.end(), saftey_inquiry.begin(), ::tolower);
    if (saftey_inquiry == "y" || saftey_inquiry == "yes") {
        safe=saftey(available, need, allocation, resources, processes,0, 0, 0); 
    }

 // requests.
    string request_inquiry;
    cout << "Do you have request? [Yes/No]" << endl;
    cin >> request_inquiry;
    transform(request_inquiry.begin(), request_inquiry.end(), request_inquiry.begin(), ::tolower);
    if (request_inquiry == "y" || request_inquiry == "yes") {
        request(available, need, allocation, resources, processes);
    }
    system("pause");
}
// saftey function checks if there is a safe state and creates correct sequence. 
bool saftey(vector<int> available, vector<vector<int>> need, vector<vector<int>> allocation, int resources, int processes,int process_requested, int func_flag, int request_flag) {
   
    vector <int> work(resources, 0);
    work = available;
    vector <bool> finish(processes, false);
    vector <int> sequence(processes);
    bool safe = false;
    int flag = 0 , needflag=0;
    for (int k = 0;k < processes; k++) {
        for (int i = 0; i < processes; i++) {
            for (int j = 0; j < resources; j++) {
                if (need[i][j] <= work[j])
                    needflag++;
            }
            if (finish[i] == false && needflag == resources) {
                finish[i] = true;
                sequence[flag] = i;
                for (int j = 0; j < resources; j++) {
                    work[j] = work[j] + allocation[i][j];
                }
                flag++;
            }
            needflag = 0;
        }
    }
    if (flag != processes) {
        cout << "Not safe" <<endl;
        return false;
    }
    else {
        safe = true;
        sequence_print(sequence,safe, processes,func_flag, process_requested,request_flag,resources);
        return true;
    }
}
// sequence_print prints the sequence of safe state.
void sequence_print(vector <int> sequence, bool safe , int processes, int func_flag,int process_requested, int request_flag, int resources)
{
    if (func_flag == 0) {
        cout << "Yes, safe state <";
        for (int j = 0; j < processes; j++) {
            cout << "P" << sequence[j];
            if (j == processes - 1) {
                cout << ">" <<endl;
            }
            else {
                cout << " ";
            }
        }
    }
    else {
        cout << "Yes request can be granted with safe state, Safe state <P"<<process_requested<<"req ";
        for (int j = 0; j < processes; j++) {
            if (sequence[j] == process_requested && request_flag == resources) {
                continue;
            }
                cout << "P" << sequence[j];
                if (j == processes - 1) {
                    cout << ">" << endl;
                }
                else {
                    cout << " ";
            }
        }
    }
}
// immediate request function.
void request(vector<int> available, vector<vector<int>> need, vector<vector<int>> allocation, int resources, int processes) {
    cout << "Process number:";
    int process_requested;
    cin >> process_requested;
    vector <int> request_in(resources);
    bool need_approval = false;
    bool safe = false;
    bool request_finish = false;
    int request_flag=0;
    cout << "Requested resources for P" << process_requested << ":" << endl;
    for (int i = 0; i < resources; i++) {
        cin >> request_in[i];
    }

    if (request_in <= need[process_requested]) {
        need_approval = true;
    }
    else {
        cout << "Maximum claim exceeded" << endl;
        return;
    }
    if (need_approval && request_in <= available)
    {
        for (int i = 0; i < resources; i++) {
            available[i] -= request_in[i];
            allocation[process_requested][i] += request_in[i];
            need[process_requested][i] -= request_in[i];
        }
        
    for (int j = 0; j < resources; j++) {
        if (need[process_requested][j] == 0) {
            request_flag++;
        }
    }
        safe = saftey(available, need, allocation, resources, processes,process_requested,1, request_flag);
    }
    if (!safe) {
        for (int i = 0; i < resources; i++) {
            available[i] += request_in[i];
            allocation[process_requested][i] -= request_in[i];
            need[process_requested][i] += request_in[i];

        }
        cout << "Request can not be granted immediatly";
    }
    return;
}
// matrices_input takes allocation, max, and available matrices from user. 
void matrices_input(vector<int> &available, vector<vector<int>> &need, vector<vector<int>> &allocation, vector<vector<int>> &Max_resources, int resources, int processes) {
    for (int i = 0; i < processes; i++) {
        cout << "Allocation of resources for process" << i << endl;
        for (int j = 0; j < resources; j++)
        {
            cin >> allocation[i][j];
        }
    }
    for (int i = 0; i < processes; i++) {
        cout << "Max resources for process" << i << endl;
        for (int j = 0; j < resources; j++)
        {
            cin >> Max_resources[i][j];
        }
    }

    cout << "Enter available resources:" << endl;
    for (int j = 0; j < resources; j++)
    {
        cin >> available[j];
    }
    cout << "Need matrix:" << endl;
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++)
        {

            need[i][j] = Max_resources[i][j] - allocation[i][j];
            cout << need[i][j] << " ";
        }

        cout << endl;
    }
}


