/*
MIT License

Copyright (c) 2017 TrolleY

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/**
*	@file xlinq_lookup.h
*	Traversing collection and grouping it by keys.
*	@author TrolleY
*/
#ifndef XLINQ_LOOKUP_H_
#define XLINQ_LOOKUP_H_

#include "xlinq_base.h"
#include "xlinq_exception.h"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <list>
#include <utility>

namespace xlinq
{
	/*@cond XLINQ_INTERNAL*/
	namespace internal
	{
		template<typename TKeySelector, typename TKey, typename TElem, typename THasher, typename TEqComp>
		class Lookup
		{
		private:
			std::shared_ptr<IEnumerator<TElem>> _enumerator;
			TKeySelector _keySelector;
			THasher _hasher;
			TEqComp _eqComp;
			std::unordered_map<TKey, std::shared_ptr<std::list<TElem>>, THasher, TEqComp> _groupedItems;
			std::list<TKey> _foundKeys;
			bool _started;
			bool _finished;

		public:
			Lookup(std::shared_ptr<IEnumerator<TElem>> enumerator, TKeySelector keySelector, THasher hasher, TEqComp eqComp)
				: _enumerator(enumerator), _keySelector(keySelector), _hasher(hasher), _eqComp(eqComp),
				_groupedItems(std::unordered_map<TKey, std::shared_ptr<std::list<TElem>>, THasher, TEqComp>()),
				_foundKeys(std::list<TKey>()),
				_started(false), _finished(false)
			{}

			THasher getHasher() const { return _hasher; }

			TEqComp getComparer() const { return _eqComp; }

			bool next(bool keyStep = true, TKey* aKey = NULL, TElem* anElem = NULL)
			{
				_started = true;
				while (_enumerator->next())
				{
					auto elem = _enumerator->current();
					auto key = _keySelector(elem);
					auto it = _groupedItems.find(key);
					if (it == _groupedItems.end())
					{
						auto pair = std::make_pair(key, std::shared_ptr<std::list<TElem>>(new std::list<TElem>()));
						pair.second->push_back(elem);
						_groupedItems.insert(pair);
						_foundKeys.push_back(key);
						if (aKey)
							*aKey = key;
						if (anElem)
							*anElem = elem;
						return true;
					}
					else
					{
						(*it).second->push_back(elem);
						if (!keyStep)
						{
							if (aKey)
								*aKey = key;
							if (anElem)
								*anElem = elem;
							return true;
						}
					}
				}
				_finished = true;
				return false;
			}

			bool finished() { return _finished; }

			typename std::list<TKey>::const_iterator beginKeyIterator() { return _foundKeys.begin(); }

			typename std::list<TKey>::const_iterator endKeyIterator() { return _foundKeys.end(); }

		private:

			std::shared_ptr<std::list<TElem>> getElementsFor(TKey key)
			{
				auto it = _groupedItems.find(key);
				if (it == _groupedItems.end())
				{
					TKey foundKey;
					while (next(true, &foundKey))
					{
						if (_eqComp(foundKey, key))
						{
							it = _groupedItems.find(key);
							break;
						}
					}
					throw KeyNotFoundException();
				}
				return (*it).second;
			}
		public:

			typename std::list<TElem>::const_iterator begin(TKey key)
			{
				return getElementsFor(key)->begin();
			}

			typename std::list<TElem>::const_iterator end(TKey key)
			{
				return getElementsFor(key)->end();
			}
		};
	}
	/*@endcond*/
}

#endif