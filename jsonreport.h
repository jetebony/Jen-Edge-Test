#ifndef CJSONREPORT_H
#define CJSONREPORT_H

#include "def.h"

#include <json-c/json.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "stringutils.h"

using namespace std;

namespace integra_utils
{
    class LIBRARY_EXPORT CJsonReport
    {
    public:

        class CError
        {
        public:

            CError()                                  { }
            CError(const CError &_err)                { copy(_err);     }

            void copy(const CError &_err);

            const string &message()     const          { return mMsg;     }
            const string &description() const          { return mDesc;    }

            void setMessage(const     string &_msg)    { mMsg   = _msg;   }
            void setDescription(const string &_desc)   { mDesc  = _desc;  }

            json_object *toJSON() const;

            CError &operator=(const CError &_err) { copy(_err); return *this; }

        private:

            string mMsg;
            string mDesc;
        };

        class CWarning
        {
        public:

            CWarning() : mLevel(-1)                    { }
            CWarning(const CWarning &_warn)            { copy(_warn);     }

            void copy(const CWarning &_warn);

            const string &message()     const          { return mMsg;     }
            const string &description() const          { return mDesc;    }
            int           level()       const          { return mLevel;   }

            void setMessage(const     string &_msg)    { mMsg   = _msg;   }
            void setDescription(const string &_desc)   { mDesc  = _desc;  }
            void setLevel(int _level)                  { mLevel = _level; }

            json_object *toJSON() const;

            CWarning &operator=(const CWarning &_warn) { copy(_warn); return *this; }

        private:

            string mMsg;
            string mDesc;
            int    mLevel;
        };

    public:

        typedef map<string,string>             TStringData;
        typedef map<string,TStringList>        TStringListData;
        typedef map<string,int>                TIntData;
        typedef map<string,bool>               TBoolData;
        typedef map<string,double>             TDoubleData;
        typedef pair<string,string>            TStringPair;
        typedef vector<TStringPair>            TStringPairList;
        typedef map<string,TStringPairList>    TStringPairData;

    public:

        CJsonReport();
        CJsonReport(const CJsonReport &_rpt)              { copy(_rpt); }
        CJsonReport(bool _success) : mSuccess(_success)   { }
        ~CJsonReport();

        void clear();
        void copy(const CJsonReport &_rpt);

        void addError(const   string &_msg, const string &_desc);
        void addWarning(const string &_msg, const string &_desc, int _level);
        void addData(const    string &_data)               { mData    = _data;      }
        void setSuccess(bool      _isSuccess)              { mSuccess = _isSuccess; }

        void addData(const string &_key, const string           &_val);
        void addData(const string &_key, int                     _val);
        void addData(const string &_key, bool                    _val);
        void addData(const string &_key, double                  _val);
        void addData(const string &_key, const TStringPairList  &_val);
        void addData(const string &_key, const TStringList      &_val);

        size_t addObject();
        size_t nmrObject()      const { return mObjects.size(); }
        size_t selectedObject() const { return mSelectedObject; }
        bool   selectObject(size_t _idx);

        string toString() const;

        CJsonReport &operator=(const CJsonReport &_rpt) { copy(_rpt); return *this; }

    private:

        typedef vector<CWarning>           TWarnings;
        typedef vector<CError>             TErrors;

        struct SObject
        {
            TStringData     mStringData;
            TIntData        mIntData;
            TBoolData       mBoolData;
            TDoubleData     mDoubleData;
            TStringPairData mStringPairData;
            TStringListData mStringListData;
        };

        typedef vector<SObject> TObjects;

        size_t          mSelectedObject;
        TWarnings       mWarnings;
        TErrors         mErrors;
        TObjects        mObjects;
        string          mData;

        bool            mSuccess;
    };

}
#endif // CJSONREPORT_H
