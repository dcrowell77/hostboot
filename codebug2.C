/*
 Which of these code snippets is wrong and why?
 
 Feel free to ask questions
 
 i_msgQ is entry we would like to delete from the iv_regMsgQ vector
 */

typedef std::vector<regMsgQ_t> EventRegistry_t; 


// A
for(EventRegistry_t::iterator r = iv_regMsgQ.begin();
    r != iv_regMsgQ.end();
    ++r)
{
    // erase all instances
    if(r->msgQ == i_msgQ)
    {
        TRACFCOMP(g_trac_initsvc,
                  "InitService::unregisterShutdownEvent: "
                  "componentID=0x%x, queue=0x%x, msgType=0x%x, priority=0x%x",
                  r->compID, r->msgQ, r->msgType, r->msgPriority);

        result = true;
        iv_regMsgQ.erase(r);
    }
}

// B
for(EventRegistry_t::iterator r = iv_regMsgQ.begin();
    r != iv_regMsgQ.end();)
{
    // erase all instances
    if(r->msgQ == i_msgQ)
    {
        TRACFCOMP(g_trac_initsvc,
                  "InitService::unregisterShutdownEvent: "
                  "componentID=0x%x, queue=0x%x, msgType=0x%x, priority=0x%x",
                  r->compID, r->msgQ, r->msgType, r->msgPriority);

        result = true;
        r = iv_regMsgQ.erase(r);
    }
    else
    {
        ++r;
    }
}