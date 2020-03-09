#include <iostream>
#include <map>
#include "OfficeHour.h"

/*
Given the current Office Hour queue and the currently on-duty staffers, return
how long the nth student will need to wait before a Staffer can reach them
Return -1 if there are no staff available to help the nth student

@param queue A queue containing the students currently waiting for help
@param onDutyStaff  A vector of Staffers currently on duty
@param nthStudent   The student for whom we are calculating a wait time
@return             How many minutes the nth student will have to wait before getting help
*/
int waitTime(queue<Student> queue, vector<Staffer> onDutyStaff, int nthStudent)
{
    if (onDutyStaff.size() == 0)
    {
        return -1;
    }
    vector<int> busyUntil(onDutyStaff.size(), 0);
    for (size_t i = 0; i < busyUntil.size(); i++)
    {
        if (onDutyStaff[i].getEnergyLevel() <= 0)
        {
            busyUntil[i] = 112358;
        }
    }
    if (*min_element(busyUntil.begin(), busyUntil.end()) == 112358)
    {
        return -1;
    }
    Student currentStudent = queue.front();
    queue.pop();
    int currentStafferIdx = distance(busyUntil.begin(), min_element(busyUntil.begin(), busyUntil.end()));
    int timeUsed = 0;
    Staffer currentStaffer = onDutyStaff[currentStafferIdx];
    timeUsed = currentStaffer.answerQuestion(currentStudent.getQuestionType(), currentStudent.getTimeNeeded());
    if (timeUsed == currentStudent.getTimeNeeded())
    {
        if (currentStaffer.getEnergyLevel < 5)
        {
            currentStafferIdx = distance(busyUntil.begin(), min_element(busyUntil.begin() + currentStafferIdx, busyUntil.end()));
                }
    }

    while ()
    {
    }
    cout << "< " << busyUntil[0] << " , " << busyUntil[1] << " >" << endl;
    return -1;
}
