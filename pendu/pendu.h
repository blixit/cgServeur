/* 
 * File:   pendu.h
 * Author: blixit
 *
 * Created on 5 août 2016, 19:45
 */

#ifndef PENDU_H
#define PENDU_H

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

const string NET_PARAM_START = "start"; 
const string NET_PARAM_END = "end"; 
const string NET_PARAM_NBTRIES = "nbTries"; 
const string NET_PARAM_TRYW = "tw"; //try world
const string NET_PARAM_TRYL = "tl"; //try letter
const string NET_PARAM_NOTIFY = "notif"; 
const string NET_PARAM_WORDUPDATED = "upword"; 


#endif /* PENDU_H */

