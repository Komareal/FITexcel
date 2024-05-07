#include "CASTNodePtr.h"

CASTNodePtr::CASTNodePtr(): m_ptr(nullptr) {
}

CASTNodePtr::CASTNodePtr(std::shared_ptr<AASTNode> &&ptr): m_ptr(std::move(ptr)) {
}

CASTNodePtr::CASTNodePtr(const CASTNodePtr &other): m_ptr(other.m_ptr) {
}

CASTNodePtr::CASTNodePtr(CASTNodePtr &&other) noexcept: m_ptr(std::move(other.m_ptr)) {
}

CASTNodePtr &CASTNodePtr::operator=(CASTNodePtr other) {
    std::swap(other.m_ptr, m_ptr);
    return *this;
}
