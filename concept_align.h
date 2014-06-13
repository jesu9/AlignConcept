#ifndef CONCEPT_ALIGN_H_
#define CONCEPT_ALIGN_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

namespace aladdin   {
class BaseConcept   {
public:
    BaseConcept(int dim): dim_(dim){}
    void SetConcept(const std::vector<double>& concept_vec);
    std::string ToString();
public:
    int dim_;
    std::vector<double> concept_vec_;
};

class UvAConcept
    : public BaseConcept    {
public:
    UvAConcept(int dim, int frame): BaseConcept(dim), frame_(frame){}
public:
    int frame_;
};

class ActionConcept
    : public BaseConcept    {
public:
    ActionConcept(int dim, int start_frame, int end_frame):
        BaseConcept(dim), start_frame_(start_frame),
        end_frame_(end_frame), aligned_(false){}
    bool Align(const std::vector<UvAConcept*>& uva);
public:
    int start_frame_;
    int end_frame_;
    bool aligned_;
};
}   // namespace aladdin


#endif // CONCEPT_ALIGN_H_
