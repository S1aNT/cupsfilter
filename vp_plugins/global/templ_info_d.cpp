#include <templ_info_d.h>

Templ_info_d::Templ_info_d()
    : t_ver(0)
{
    date_time.clear();
    t_author.clear();
    t_name.clear();
    t_desc.clear();
    p_size.clear();
    page_orient = false;
    page_width = 0.0;
    page_height = 0.0;
    m_top = 0.0;
    m_bottom = 0.0;
    m_left = 0.0;
    m_right = 0.0;
    firstPageElemCount  = 0;
    secondPageElemCount = 0;
    thirdPageElemCount  = 0;
    fourthPageElemCount = 0;
}

Templ_info_d::Templ_info_d (const Templ_info_d& other)
    : QSharedData(other),
    date_time(other.date_time),
    t_author(other.t_author),
    t_name(other.t_name),
    t_desc(other.t_desc),
    p_size(other.p_size),
    page_orient(other.page_orient),
    page_width(other.page_width),
    page_height(other.page_height),
    m_top(other.m_top),
    m_bottom(other.m_bottom),
    m_left(other.m_left),
    m_right(other.m_right),
    firstPageElemCount(other.firstPageElemCount),
    secondPageElemCount(other.secondPageElemCount),
    thirdPageElemCount(other.thirdPageElemCount),
    fourthPageElemCount(other.fourthPageElemCount)

{

}

