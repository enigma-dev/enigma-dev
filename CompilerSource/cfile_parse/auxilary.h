/**  Copyright (C) 2008 Josh Ventura
**  This file is a part of the ENIGMA Development Environment.
**  It is released under the GNU General Public License, version 3.
**  See the license of local files for more details.
**/
#define	const		__const
#define	inline		__inline
#define	signed		__signed
#define	volatile	__volatile
  
  template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator
    {
      /// One of the @link iterator_tags tag types@endlink.
      typedef _Category  iterator_category;
      /// The type "pointed to" by the iterator.
      typedef _Tp        value_type;
      /// Distance between iterators is represented as this type.
    };

  template<typename _Iterator>
    struct iterator_traits
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

  template<typename _Tp>
    struct iterator_traits<_Tp*>
    {
      typedef _Tp                         value_type;
      typedef _Tp*                        pointer;
      typedef _Tp&                        reference;
    };

  template<typename _Tp>
    struct iterator_traits<const _Tp*>
    {
      typedef _Tp                         value_type;
      typedef const _Tp*                  pointer;
      typedef const _Tp&                  reference;
    };
    
#error Ass caekz

