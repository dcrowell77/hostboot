/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/include/usr/targeting/common/iterators/targetiterator.H $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2011,2014              */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

#ifndef __TARGETING_COMMON_TARGETITERATOR_H
#define __TARGETING_COMMON_TARGETITERATOR_H

/**
 *  @file targeting/common/iterators/targetiterator.H
 *
 *  @brief Interface describing iterator/const iterator used to iterate through
 *      target service targets
 */

//******************************************************************************
// Includes
//******************************************************************************

// STD
#include <stddef.h>

// Other Host Boot Components
#include <builtins.h>

// Targeting Component

//******************************************************************************
// Macros
//******************************************************************************

#undef TARG_NAMESPACE
#undef TARG_CLASS
#undef TARG_FUNC

//******************************************************************************
// Interface
//******************************************************************************

namespace TARGETING
{

#define TARG_NAMESPACE "TARGETING::"

#define TARG_CLASS "_TargetIterator<T>::"

class Target;

/**
 *  @brief Class which iterates through targets managed by the target service.
 *      Provides "Target*" and "const Target*" versions via templates
 */
template<typename T>
class _TargetIterator
{
    public:

        /**
         *  @brief Maps type of iterated element to common alias (Target* or
         *      const Target*)
         */
        typedef T iterator;
        typedef T value_type;

        /**
         *  @brief Create an iterator to a (const/non-const) target handle.
         *      Defaults to end()
         */
        ALWAYS_INLINE inline
        _TargetIterator()
            : iv_pCurrent(NULL)
        {
        }

        /**
         *  @brief Create an iterator to a (const/non-const) target handle
         *
         *  @param[in] i_pTarget Target handle (pointer or const pointer
         *      depending on flavor) the iterator should reference
         *
         *  @note User should not assign a hidden system target to the iterator,
         *      as it does not support hidden targets
         */
        ALWAYS_INLINE inline
        explicit _TargetIterator(T i_pTarget)
            : iv_pCurrent(i_pTarget)
        {
        }

        /**
         *  @brief Destroy an iterator to a (const/non-const) target handle
         *
         *  @note Iterator does not own any resources to destroy
         */
        ALWAYS_INLINE inline
        ~_TargetIterator()
        {
        }

        /**
         *  @brief Dereference the iterator (return what it points to)
         *
         *  @return Target handle in form of Target* or const Target*
         *
         *  @note Returns NULL if it points to end(), should not be
         *      dereferenced in this case.
         */
        ALWAYS_INLINE inline
        T operator* () const
        {
            // Only return copy of the target pointer (instead of a reference)
            // because a reference will go out of scope when the iterator
            // advances
            return iv_pCurrent;
        }

        /**
         *  @brief Dereference the iterator (return what it points to)
         *      and (outside this function) call a function/member of
         *      the referenced target
         *
         *  @return Target handle in form of Target* or const Target*
         *
         *  @note Returns NULL if it points to end(), causing an
         *      assert when automatically dereferenced.
         */
        ALWAYS_INLINE inline
        T operator->() const
        {
            // Only return copy of the target pointer (instead of a reference)
            // because a reference will go out of scope when the iterator
            // advances
            return iv_pCurrent;
        }

        /**
         *  @brief Pre-increment the iterator
         *
         *  @return The reference to the same iterator after advancing it
         */
        ALWAYS_INLINE inline
        _TargetIterator& operator++();

        /**
         *  @brief Post-increment the iterator
         *
         *  @param[in] UNNAMED Dummy parameter used to distinguish
         *      this interface from pre-increment
         *
         *  @return Copy of the original iterator before it advanced
         */
        ALWAYS_INLINE inline
        _TargetIterator operator++(int);

        /**
         *  @brief Determine if two iterators of same type are logically equal
         *
         *  @param[in] i_rhs The other iterator to compare
         *
         *  @return bool indicating whether the iterators point to the same
         *      entity
         *
         *  @retval true Iterators point to same entity
         *  @retval false Iterators do not point to same entity
         */
        ALWAYS_INLINE inline
        bool operator==(const _TargetIterator& i_rhs) const
        {
            return (iv_pCurrent == i_rhs.iv_pCurrent);
        }

        /**
         *  @brief Determine if two iterators of same type are logically not
         *      equal
         *
         *  @param[in] i_rhs The other iterator to compare
         *
         *  @return bool indicating whether the iterators point to a different
         *      logical entity
         *
         *  @retval true Iterators point to different entity
         *  @retval false Iterators point to same entity
         */
        ALWAYS_INLINE inline
        bool operator!=(const _TargetIterator& i_rhs) const
        {
            return (iv_pCurrent != i_rhs.iv_pCurrent);
        }

        /**
         *  @brief Assignment operator; assign iterator to another (such that
         *      they logically point to same entity)
         *
         *  @param[in] i_rhs The iterator to assign
         *
         *  @return Reference to the iterator, now updated to point to the same
         *      entity as the input iterator points to
         */
        ALWAYS_INLINE inline
        _TargetIterator& operator=(const _TargetIterator& i_rhs)
        {
            iv_pCurrent = i_rhs.iv_pCurrent;
            return *this;
        }

        /**
         *  @brief Copy constructor; assign iterator to a new iterator (such
         *      that they logically point to same entity)
         *
         *  @param[in] i_rhs The iterator to assign
         */
        ALWAYS_INLINE inline
        _TargetIterator(const _TargetIterator& i_rhs)
         :   iv_pCurrent(i_rhs.iv_pCurrent)
        {
        }

    private:

        /**
         *  @brief Advance the iterator to point to the next item maintained by
         *      the target service (or end() if end of list)
         */
        void advance();

    protected:

        T iv_pCurrent; // Pointer to current target
};

/**
 *  @brief Type aliases to simplify user code
 */
typedef _TargetIterator<Target*> TargetIterator;
typedef _TargetIterator<const Target*> ConstTargetIterator;

//******************************************************************************
// _TargetIterator::operator++ (postincrement)
//******************************************************************************

template<typename T>
_TargetIterator<T> _TargetIterator<T>::operator++(int)
{
    _TargetIterator l_originalIterator(*this);
    advance();
    return l_originalIterator;
}

//******************************************************************************
// _TargetIterator::operator++ (preincrement)
//******************************************************************************

template<typename T>
_TargetIterator<T>& _TargetIterator<T>::operator++()
{
    advance();
    return *this;
}

#undef TARG_CLASS
#undef TARG_NAMESPACE

} // End namespace TARGETING

#endif // __TARGETING_COMMON_TARGETITERATOR_H

