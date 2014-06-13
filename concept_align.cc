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

// scan the vector of uva concepts, average pool all the frames
// fall between begin_frame_ and end_frame_, and updates the
// concept_vec_ and aligned
// returns false if no frame fall within the range
bool ActionConcept::Align(const vector<UvAConcept*>& uva)    {
    if (concept_vec_.size() < 1 || uva.size() < 1)
        return false;
    vector<double> ave_vec(uva[0]->dim_, 0.0);
    int counter = 0;
    for (int i = 0; i < uva.size(); ++i)    {
        if (uva[i]->frame_ < start_frame_ ||
            uva[i]->frame_ > end_frame_ ||
            uva[i]->concept_vec_.size() != uva[i]->dim_) {
            continue;
        }
        for (int j = 0; j < uva[i]->dim_; ++j)   {
            ave_vec[j] += uva[i]->concept_vec_[j];
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
