#ifndef XLINQ_TAKE_H_
#define XLINQ_TAKE_H_

#include "xlinq_base.h"

namespace xlinq
{
	template<typename TElem>
	class _TakeEnumerable : public IEnumerable<TElem>
    {
    private:
        std::shared_ptr<IEnumerable<TElem>> _enumerable;
        int _num;
    public:
        _TakeEnumerable(std::shared_ptr<IEnumerable<TElem>> enumerable, int num)
            : _enumerable(enumerable), _num(num)
        {}

        class _TakeEnumerator : public IEnumerator<TElem>
        {
        private:
            std::shared_ptr<IEnumerator<TElem>> _enumerator;
            int _num;
        public:
            _TakeEnumerator(std::shared_ptr<IEnumerator<TElem>> enumerator, int num)
                : _enumerator(enumerator), _num(num)
            {}

            virtual bool next() override
            {
                while(_num > 0)
                {
                    if (_enumerator->next())
                    {
                        _num--;
                        return true;
                    }
                    _num = 0;
                }
                return false;
            }

            virtual TElem current() override
            {
                return _enumerator->current();
            }
        };

        virtual std::shared_ptr<IEnumerator<TElem>> getEnumerator() override
        {
            return std::shared_ptr<IEnumerator<TElem>>(new _TakeEnumerable::_TakeEnumerator(_enumerable->getEnumerator(), _num));
        }
    };

    class _TakeBuilder
    {
    private:
        int _num;
    public:
        _TakeBuilder(int num) : _num(num) {}

        template<typename TElem>
        decltype(auto) build(std::shared_ptr<IEnumerable<TElem>> enumerable)
        {
            return std::shared_ptr<IEnumerable<TElem>>(new _TakeEnumerable<TElem>(enumerable, _num));
        }
    };

    decltype(auto) take(int num)
    {
        return _TakeBuilder(num);
    }
}

#endif
