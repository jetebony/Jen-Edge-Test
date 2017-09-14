#include "jsonreport.h"

namespace integra_utils
{
    CJsonReport::CJsonReport()
        : mSelectedObject(-1)
    {   }

    CJsonReport::~CJsonReport()
    {   }

    void CJsonReport::clear()
    {
        mWarnings.clear();
        mErrors.clear();
        mData.clear();
        TObjects::iterator it = mObjects.begin();
        while (it != mObjects.end())
        {
            (*it).mStringData.clear();
            (*it).mIntData.clear();
            (*it).mBoolData.clear();
            (*it).mDoubleData.clear();
            (*it).mStringPairData.clear();
            ++it;
        }
        mObjects.clear();
        mSuccess = false;
        mSelectedObject = -1;
    }

    void CJsonReport::copy(const CJsonReport &_rpt)
    {
        mWarnings       = _rpt.mWarnings;
        mErrors         = _rpt.mErrors;
        mData           = _rpt.mData;
        mSuccess        = _rpt.mSuccess;
        mObjects        = _rpt.mObjects;
        mSelectedObject = _rpt.mSelectedObject;
    }

    void CJsonReport::addError(const   string &_msg, const string &_desc)
    {
        CError error;
        error.setMessage(_msg);
        error.setDescription(_desc);

        mErrors.push_back(error);
    }

    void CJsonReport::addWarning(const string &_msg, const string &_desc, int _level)
    {
        CWarning warn;
        warn.setMessage(_msg);
        warn.setDescription(_desc);
        warn.setLevel(_level);

        mWarnings.push_back(warn);
    }

    void CJsonReport::addData(const string &_key, const string &_val)
    {
        if (mObjects.size() > mSelectedObject)
            mObjects.at(mSelectedObject).mStringData[_key] = _val;
    }

    void CJsonReport::addData(const string &_key, int _val)
    {
        if (mObjects.size() > mSelectedObject)
            mObjects.at(mSelectedObject).mIntData[_key] = _val;
    }

    void CJsonReport::addData(const string &_key, bool _val)
    {
        if (mObjects.size() > mSelectedObject)
            mObjects.at(mSelectedObject).mBoolData[_key] = _val;
    }

    void CJsonReport::addData(const string &_key, double _val)
    {
        if (mObjects.size() > mSelectedObject)
            mObjects.at(mSelectedObject).mDoubleData[_key] = _val;
    }

    void CJsonReport::addData(const string &_key, const TStringPairList &_val)
    {
        if (mObjects.size() > mSelectedObject)
            mObjects.at(mSelectedObject).mStringPairData[_key] = _val;
    }

    void CJsonReport::addData(const string &_key, const TStringList &_val)
    {
        if (mObjects.size() > mSelectedObject)
            mObjects.at(mSelectedObject).mStringListData[_key] = _val;
    }

    size_t CJsonReport::addObject()
    {
        SObject obj;
        mObjects.push_back(obj);

        mSelectedObject = mObjects.size() - 1;
        return mSelectedObject;
    }

    bool CJsonReport::selectObject(size_t _idx)
    {
        if (nmrObject() > _idx)
            mSelectedObject = _idx;

        return mSelectedObject == _idx;
    }

    string CJsonReport::toString() const
    {
        json_object *json     = json_object_new_object();
        json_object *success  = json_object_new_boolean(mSuccess);
        json_object *errors   = json_object_new_array();
        json_object *warnings = json_object_new_array();
        json_object *data     = json_object_new_array();

        TErrors::const_iterator eit = mErrors.begin();
        while (eit != mErrors.end())
        {
            json_object *error = json_object_new_object();
            json_object *msg   = json_object_new_string(eit->message().c_str());
            json_object *desc  = json_object_new_string(eit->description().c_str());

            json_object_object_add(error, "message",     msg);
            json_object_object_add(error, "description", desc);
            json_object_array_add(errors, error);

            ++eit;
        }

        TWarnings::const_iterator wit = mWarnings.begin();
        while (wit != mWarnings.end())
        {
            json_object *warning = json_object_new_object();
            json_object *msg     = json_object_new_string(wit->message().c_str());
            json_object *desc    = json_object_new_string(wit->description().c_str());
            json_object *level   = json_object_new_int(wit->level());

            json_object_object_add(warning, "message",     msg);
            json_object_object_add(warning, "description", desc);
            json_object_object_add(warning, "level",       level);
            json_object_array_add(warnings, warning);

            ++wit;
        }

        bool objWithData = false;
        TObjects::const_iterator it = mObjects.begin();
        while (it != mObjects.end())
        {
            if (((*it).mStringData.size()     > 0) ||
                ((*it).mStringListData.size() > 0) ||
                ((*it).mStringPairData.size() > 0) ||
                ((*it).mIntData.size()        > 0) ||
                ((*it).mDoubleData.size()     > 0) ||
                ((*it).mBoolData.size()       > 0))
            {
                objWithData = true;
                json_object *obj = json_object_new_object();
                json_object_array_add(data, obj);

                TStringData::const_iterator sit = (*it).mStringData.begin();
                while (sit != (*it).mStringData.end())
                {
                    json_object *str = json_object_new_string(sit->second.c_str());
                    json_object_object_add(obj, sit->first.c_str(), str);
                    ++sit;
                }

                if ((*it).mStringListData.size() > 0)
                {
                    json_object *list     = json_object_new_array();
                    TStringListData::const_iterator slit = (*it).mStringListData.begin();
                    while (slit != (*it).mStringListData.end())
                    {
                        TStringList::const_iterator stri = slit->second.begin();
                        while (stri != slit->second.end())
                        {
                            json_object *str = json_object_new_string((*stri).c_str());
                            json_object_array_add(list, str);
                            ++stri;
                        }

                        json_object_object_add(obj, slit->first.c_str(), list);
                        ++slit;
                    }
                }

                TIntData::const_iterator iit = (*it).mIntData.begin();
                while (iit != (*it).mIntData.end())
                {
                    json_object *val = json_object_new_int(iit->second);
                    json_object_object_add(obj, iit->first.c_str(), val);
                    ++iit;
                }

                TBoolData::const_iterator bit = (*it).mBoolData.begin();
                while (bit != (*it).mBoolData.end())
                {
                    json_object *val = json_object_new_boolean(bit->second);
                    json_object_object_add(obj, bit->first.c_str(), val);
                    ++bit;
                }

                TDoubleData::const_iterator dit = (*it).mDoubleData.begin();
                while (dit != (*it).mDoubleData.end())
                {
                    json_object *val = json_object_new_double(dit->second);
                    json_object_object_add(obj, dit->first.c_str(), val);
                    ++dit;
                }

                TStringPairData::const_iterator spit = (*it).mStringPairData.begin();
                while (spit != (*it).mStringPairData.end())
                {
                    json_object *sp = json_object_new_object();
                    TStringPairList list = spit->second;
                    TStringPairList::const_iterator split = list.begin();
                    while (split != list.end())
                    {
                        TStringPair strpair = *split;

                        json_object *val = json_object_new_string(strpair.second.c_str());
                        json_object_object_add(sp, strpair.first.c_str(), val);
                        ++split;
                    }

                    json_object_object_add(obj, spit->first.c_str(), sp);
                    ++spit;
                }

            }
            ++it;
        }

        if (!objWithData)
        {
            json_object *pl = json_object_new_string(mData.c_str());
            json_object_array_add(data, pl);
        }

        json_object_object_add(json, "success",  success);
        json_object_object_add(json, "errors",   errors);
        json_object_object_add(json, "warnings", warnings);
        json_object_object_add(json, "data",     data);

        string jStr = json_object_to_json_string(json);

        json_object_put(json);

        return jStr;
    }

    //---------------------  CJsonReport::CError ---------------------------------

    void CJsonReport::CError::copy(const CError &_err)
    {
        mMsg   = _err.mMsg;
        mDesc  = _err.mDesc;
    }

    json_object *CJsonReport::CError::toJSON() const
    {
        json_object *err   = json_object_new_object();

        json_object *msg   = json_object_new_string(mMsg.c_str());
        json_object *desc  = json_object_new_string(mDesc.c_str());

        json_object_object_add(err, "message",     msg);
        json_object_object_add(err, "description", desc);

        return err;
    }

    //---------------------  CJsonReport::CWarning ---------------------------------

    void CJsonReport::CWarning::copy(const CWarning &_warn)
    {
        mMsg   = _warn.mMsg;
        mDesc  = _warn.mDesc;
        mLevel = _warn.mLevel;
    }

    json_object *CJsonReport::CWarning::toJSON() const
    {
        json_object *warn = json_object_new_object();

        json_object *msg   = json_object_new_string(mMsg.c_str());
        json_object *desc  = json_object_new_string(mDesc.c_str());
        json_object *level = json_object_new_int(mLevel);

        json_object_object_add(warn, "message",     msg);
        json_object_object_add(warn, "description", desc);
        json_object_object_add(warn, "level",       level);

        return warn;
    }

}


