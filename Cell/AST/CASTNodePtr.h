#ifndef CASTNodePtr_H
#define CASTNodePtr_H

#include "AASTNode.h"
#include "../../header.h"

struct CASTNodePtr {
    CASTNodePtr();

    explicit CASTNodePtr(std::shared_ptr<AASTNode> &&ptr);

    std::shared_ptr<AASTNode> m_ptr;

    CASTNodePtr(const CASTNodePtr &other);

    CASTNodePtr(CASTNodePtr &&other) noexcept;


    CASTNodePtr &operator=(CASTNodePtr other);
};


#endif //CASTNodePtr_H
