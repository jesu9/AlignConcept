#include "concept_align.h"

namespace aladdin   {
using namespace std;

void BaseConcept::SetConcept(const vector<double>& concept_vec) {
    if (concept_vec.size() != dim_) {
        cout<<"Dimension not match"<<endl;
        return;
    }
    concept_vec_ = concept_vec;
}
string BaseConcept::ToString()  {
    if (concept_vec_.size() < 1)
        return "";
    stringstream ss;
    for (int i = 0; i < dim_ - 1; ++i)  {
        ss << concept_vec_[i] << " ";
    }
    ss << concept_vec_[dim_-1];
    return ss.str();
}

// scan the vector of frame-based concepts, average pool all the frames
// fall between begin_frame_ and end_frame_, and updates the
// concept_vec_ and aligned
// returns false if no frame fall within the range
bool ClipConcept::Align(const vector<FrameConcept*>& fc)    {
    if (concept_vec_.size() < 1 || fc.size() < 1)
        return false;
    vector<double> ave_vec(fc[0]->dim_, 0.0);
    int counter = 0;
    for (int i = 0; i < fc.size(); ++i)    {
        if (fc[i]->frame_ < start_frame_ ||
            fc[i]->frame_ > end_frame_ ||
            fc[i]->concept_vec_.size() != fc[i]->dim_) {
            continue;
        }
        for (int j = 0; j < fc[i]->dim_; ++j)   {
            ave_vec[j] += fc[i]->concept_vec_[j];
        }
        ++counter;
    }
    if (counter < 1)    {
        return false;
    }
    for (int i = 0; i < ave_vec.size(); ++i)    {
        ave_vec[i] /= counter;
    }
    concept_vec_.insert(concept_vec_.end(), ave_vec.begin(),
                        ave_vec.end());
    dim_ = concept_vec_.size();
    aligned_ = true;
    return true;
}

// scan the vector of clip-based concepts, average pool all the clips
// with overlap, and updates the concept_vec_
bool ClipConcept::Align(const vector<ClipConcept*>& cc) {
    if (concept_vec_.size() < 1 || cc.size() < 1)
        return false;
    vector<double> ave_vec(cc[0]->dim_, 0.0);
    int counter = 0;
    for (int i = 0; i < cc.size(); ++i) {
        if (cc[i]->start_frame_ >= end_frame_ ||
            cc[i]->end_frame_ <= start_frame_ ||
            cc[i]->concept_vec_.size() != cc[i]->dim_)  {
            continue;
        }
        for (int j = 0; j < cc[i]->dim_; ++j)   {
            ave_vec[j] += cc[i]->concept_vec_[j];
        }
        ++counter;
    }
    if (counter < 1)    {
        return false;
    }
    for (int i = 0; i < ave_vec.size(); ++i)    {
        ave_vec[i] /= counter;
    }
    concept_vec_.insert(concept_vec_.end(), ave_vec.begin(),
                        ave_vec.end());
    dim_ = concept_vec_.size();
    aligned_ = true;
    return true;
}

}   // namespace aladdin
