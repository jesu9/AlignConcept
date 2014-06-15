#include <fstream>
#include <cstdlib>

#include "concept_align.h"

using namespace std;
using aladdin::ClipConcept;

int main(int argc, char **argv) {
    if (argc < 5)   {
        cout<<"Usage: "<<argv[0]<<" actionConceptFile uvaConceptFile uvaInfoFile outputFile [actionDim] [uvaDim]"<<endl;
        return 0;
    }
    vector<ClipConcept*> actionConcepts;
    vector<ClipConcept*> uvaConcepts;

    int actionDim = 101 + 51;
    int uvaDim = 1000;
    if (argc > 5)   {
        actionDim = atoi(argv[5]);
    }
    if (argc > 6)   {
        uvaDim = atoi(argv[6]);
    }
    
    ifstream fin;
    string line;
    fin.open(argv[1]);
    if (!fin.is_open()) {
        cout<<"Cannot open action concept file"<<endl;
        return 0;
    }
    // load action concept features
    int segmentCounter = 0;
    while (getline(fin, line))  {
        ClipConcept *ac = new ClipConcept(actionDim, segmentCounter*50+1, segmentCounter*50+100);
        ++segmentCounter;
        stringstream ss;
        float val;
        ss<<line;
        vector<double> vec;
        while (ss>>val) {
            vec.push_back(val);
        }
        if (vec.size() != actionDim)    {
            cout<<"Dimension not match"<<endl;
            continue;
        }
        ac->SetConcept(vec);
        actionConcepts.push_back(ac);
    }
    fin.close();

    // load uva concepts
    ifstream fin1, fin2;
    fin1.open(argv[2]);
    fin2.open(argv[3]);
    if (!fin1.is_open())    {
        cout<<"Cannot open uva concept file"<<endl;
        return 0;
    }
    if (!fin2.is_open())    {
        cout<<"Cannot open uva info file"<<endl;
        return 0;
    }
    while (getline(fin1, line)) {
        stringstream ss;
        float val;
        char comma;
        ss<<line;
        vector<double> vec;
        while (ss>>val) {
            vec.push_back(val);
            ss>>comma;
        }
        if (vec.size() != uvaDim)    {
            cout<<"Dimension not match"<<endl;
            continue;
        }
        getline(fin2, line);
        ss.clear();
        ss.str("");
        ss<<line;
        int frameNum;
        ss>>frameNum;
        ClipConcept *uc = new ClipConcept(uvaDim, frameNum, frameNum);
        // update the end_frame_ for previous clip
        if (uvaConcepts.size() > 0) {
            uvaConcepts[uvaConcepts.size()-1]->end_frame_ = frameNum;
        }
        uc->SetConcept(vec);
        uvaConcepts.push_back(uc);
    }
    if (uvaConcepts.size() > 1) {
        uvaConcepts[uvaConcepts.size()-1]->end_frame_ +=
            uvaConcepts[uvaConcepts.size()-1]->start_frame_ -
            uvaConcepts[uvaConcepts.size()-2]->start_frame_;
    }
    fin1.close();
    fin2.close();

    // save merged result
    ofstream fout;
    fout.open(argv[4]);
    if (!fout.is_open())    {
        cout<<"Cannot save result"<<endl;
        return 0;
    }
    for (int i = 0; i < actionConcepts.size(); ++i) {
        // since there might be cases where no uva concept falls within the range
        // we ignore those cases and only save those with both action and uva concepts.
        // as a reuslt, we need to store the starting frame number
        if (actionConcepts[i]->Align(uvaConcepts))  {
            fout<<actionConcepts[i]->start_frame_<<" "<<actionConcepts[i]->ToString()<<endl;
        }
    }
    fout.close();
    
    return 0;
}
