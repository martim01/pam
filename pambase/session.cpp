#include "session.h"

const wxString CH_GROUP_ORDER[8] = {L"\u2081", L"\u2082", L"\u2083", L"\u2084", L"\u2085", L"\u2086", L"\u2087", L"\u2088"};
const wxString CH_GROUP_TYPE[15] = { "Mono", "Dual Mono", "Stereo", "Lt Rt", "5.1", "7.1", "SGRP", "U01", "U02", "U03", "U04", "U05", "U06", "U07", "U08"};
const wxString CH_GROUP_CHANNEL[26] = { "M", "M1", "M2", "L", "R", "Lt", "Rt", "C", "LFE", "Ls", "Rs", "Lrs", "Rrs", "S1", "S2", "S3", "S4", "U1", "U2", "U3", "U4", "U5", "U6", "U7", "U8", "Un"};
const wxString CH_GROUP_CHANNEL_LONG[26] = { "Mono", "Mono1", "Mono2", "Left", "Right", "Left Total", "Right Total", "Center", "LFE", "Left Side", "Right Side", "Left Rear Side", "Right Rear Side", "SDI1","SDI2","SDI3", "SDI4", "Undefined1","Undefined2","Undefined3","Undefined4","Undefined5","Undefined6","Undefined7","Undefined8", "Unset"};
const wxColour CH_GROUP_COLOUR[8] = {wxColour("#fe2713"), wxColour("#67b132"), wxColour("#fccc20"), wxColour("#4525d8"), wxColour("#fb8906"), wxColour("#125ae0"), wxColour("#e4f135"), wxColour("#9d0798")};

const std::map<subsession::enumChannelGrouping, unsigned char> CH_GROUP_SIZE = {{subsession::enumChannelGrouping::M, 1}, {subsession::enumChannelGrouping::DM,2}, {subsession::enumChannelGrouping::ST,2}, {subsession::enumChannelGrouping::LtRt,2}, {subsession::enumChannelGrouping::FIVE1,6}, {subsession::enumChannelGrouping::SEVEN1,8}, {subsession::enumChannelGrouping::SGRP,4}, {subsession::enumChannelGrouping::U01,1}, {subsession::enumChannelGrouping::U02,2}, {subsession::enumChannelGrouping::U03,3}, {subsession::enumChannelGrouping::U04,4}, {subsession::enumChannelGrouping::U05,5}, {subsession::enumChannelGrouping::U06,6}, {subsession::enumChannelGrouping::U07,7}, {subsession::enumChannelGrouping::U08,8}};



wxString GetChannelLabel(const subsession::channelGrouping& ch)
{
    if(ch.nId < 8)
    {
        return CH_GROUP_CHANNEL[ch.type] + CH_GROUP_ORDER[ch.nId];
    }
    return CH_GROUP_CHANNEL[ch.type];
}

wxString GetChannelLabelLong(const subsession::channelGrouping& ch)
{
    if(ch.nId < 8)
    {
        return CH_GROUP_CHANNEL_LONG[ch.type] + CH_GROUP_ORDER[ch.nId];
    }
    return CH_GROUP_CHANNEL_LONG[ch.type];
}

wxString GetChannelGroupName(const subsession::channelGrouping& ch)
{
    return CH_GROUP_TYPE[ch.grouping]+CH_GROUP_ORDER[ch.nId];
}
