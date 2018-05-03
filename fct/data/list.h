#ifndef functional_algorithms_data_list_header
#define functional_algorithms_data_list_header

#include <vector>
#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <optional>
#include <iterator>

namespace fct
{
  using String = std::string;
  using SS = std::stringstream;
  using Bool = bool;
  using Float = float;
  using Double = long double;
  using Int = int;
  using UInt = unsigned int;
  using Char = char;
  using Long = long long;
  using ULong = unsigned long long;

  template <typename T>
  using Opt = std::optional<T>;

  template <typename T>
  using Vec = std::vector<T>;

  // template <typename S, typename T>
  template <typename ...Ts>
  using Tup = std::tuple<Ts...>;

  using std::begin;
  using std::end;
  using std::rbegin;
  using std::rend;
  using std::advance;

  // transpose :: [[T]] -> [[T]]
  template <typename T, template <typename> typename Cont>
  auto transpose( Cont<Cont<T>> const& xxs ) -> Cont<Cont<T>>
  {
    Cont<Cont<T>> out{};

    for ( uint i = 0; i < xxs.size(); i++ )
    {
      auto const& xs = xxs.at( i );
      for ( uint j = 0; j < xs.size(); j++ )
      {
        auto const& x = xs.at( j );

        if ( j >= out.size() )
          out.push_back( { x } );
        else
          out.at( j ).push_back( x );
      }
    }

    return out;
  }

  // subsets :: [T] -> [[T]]
  template <typename T, template <typename> typename Cont>
  auto subsets( Cont<T> const& xs ) -> Cont<Cont<T>>
  {
    Cont<Cont<T>> out{};
    out.reserve( std::pow( 2, xs.size() ) );

    // Add the empty set
    out.push_back( Cont<T>{} );

    for ( auto const& x : xs )
    {
      // Create copy of current subsets
      Cont<Cont<T>> new_subsets{ begin( out ), end( out ) };

      // Add element to every new subset
      for ( Cont<T>& s : new_subsets )
        s.push_back( x );

      // Add new subsets to output
      out.insert( end( out ), begin( new_subsets ), end( new_subsets ) );
    }

    return out;
  }
  // intersect :: [T] -> [T] -> [T]
  template <typename T, template <typename> typename Cont>
  auto intersect( Cont<T> const& xs, Cont<T> const& ys ) -> Cont<T>
  {
    Cont<T> out{};
    auto const& smaller = xs.size() <= ys.size() ? xs : ys;
    auto const& larger = xs.size() > ys.size() ? xs : ys;

    for ( auto const& s : smaller )
      if ( elem( s, larger ) )
        out.push_back( s );

    return out;
  }

  // union :: [T] -> [T] -> [T]
  template <typename T, template <typename> typename Cont>
  auto union_of( Cont<T> const& xs, Cont<T> const& ys ) -> Cont<T>
  {
    Cont<T> out{};
    Cont<T> const* smaller = xs.size() <= ys.size() ? &xs : &ys;
    Cont<T> const* larger = xs.size() > ys.size() ? &xs : &ys;

    for ( auto const& s : *smaller )
      if ( notElem( s, *larger ) )
        out.push_back( s );

    for ( auto const& l : *larger )
      out.push_back( l );

    return out;
  }

  // intersperse :: T -> [T] -> [T]
  template <typename T, template <typename> typename Cont>
  auto intersperse( T const& y, Cont<T> const& xs ) -> Cont<T>
  {
    if ( xs.empty() )
      return Cont<T>{};

    Cont<T> out{};

    auto a = begin( xs );
    for ( ; a < end( xs ) - 1; advance( a, 1 ) )
    {
      out.push_back( *a );
      out.push_back( y );
    }
    out.push_back( *a );

    return out;
  }

  // intercalate :: [T] -> [[T]] -> [T]
  template <typename T, template <typename> typename Cont>
  auto intercalate( Cont<T> const& xs, Cont<Cont<T>> const& xxs ) -> Cont<T>
  {
    return concat( intersperse( xs, xxs ) );
  }

  // permutations :: [T] -> [[T]]
  template <typename T, template <typename> typename Cont>
  auto permutations( Cont<T> const& xs ) -> Cont<Cont<T>>
  {
    Cont<Cont<T>> out{};
    Cont<T> mut_xs = xs;

    std::sort( begin( mut_xs ), end( mut_xs ) );

    // Factorial of xs.size() == # of permutations
    UInt size = 1;
    for (UInt i = xs.size(); i > 0; i--)
      size *= i;
    mut_xs.reserve( size );

    do
    {
      out.push_back( mut_xs );
    }
    while ( std::next_permutation( begin( mut_xs ), end( mut_xs ) ) );

    return out;
  }
  // group :: [T] -> [[T]]
  template <typename T, template <typename> typename Cont>
  auto group( Cont<T> const& xs ) -> Cont<Cont<T>>
  {
    if ( xs.empty() )
      return Cont<Cont<T>>{};

    Cont<Cont<T>> out{};

    auto a = begin( xs );
    auto b = a;

    while ( b < end( xs ) )
    {
      if ( *b == *a )
      {
        advance( b, 1 );
      }
      else
      {
        out.push_back( Cont<T>{ a, b } );
        a = b;
      }
    }
    out.push_back( Cont<T>{ a, b } );

    return out;
  }

  // inits :: [T] -> [[T]]
  template <typename T, template <typename> typename Cont>
  auto inits( Cont<T> const& xs ) -> Cont<Cont<T>>
  {
    Cont<Cont<T>> out{};

    auto i = begin( xs );

    for ( ; i < end( xs ); advance( i, 1 ) )
      out.push_back( Cont<T>{ begin( xs ), i } );
    out.push_back( Cont<T>{ begin( xs ), i } );

    return out;
  }

  // tails :: [T] -> [[T]]
  template <typename T, template <typename> typename Cont>
  auto tails( Cont<T> const& xs ) -> Cont<Cont<T>>
  {
    Cont<Cont<T>> out{};

    auto a = begin( xs );

    for ( ; a < end( xs ); advance( a, 1 ) )
      out.push_back( Cont<T>{ a, end( xs ) } );
    out.push_back( Cont<T>{} );

    return out;
  }

  // isPrefixOf :: [T] -> [T] -> Bool
  template <typename T, template <typename> typename Cont>
  auto isPrefixOf( Cont<T> const& xs, Cont<T> const& ys ) -> bool
  {
    auto ys_inits = inits( ys );
    return elem( xs, ys_inits );
  }

  // isSuffixOf :: [T] -> [T] -> Bool
  template <typename T, template <typename> typename Cont>
  auto isSuffixOf( Cont<T> const& xs, Cont<T> const& ys ) -> bool
  {
    auto ys_tails = tails( ys );
    return elem( xs, ys_tails );
  }

  // isInfixOf :: [T] -> [T] -> Bool
  template <typename T, template <typename> typename Cont>
  auto isInfixOf( Cont<T> const& xs, Cont<T> const& ys ) -> bool
  {
    if ( xs.empty() )
      return true;

    auto a = begin( ys );
    auto b = begin( ys ) + xs.size();

    Cont<T> test{ begin( xs ), end( xs ) };

    for ( ; b < end( ys ); advance( a, 1 ), advance( b, 1 ) )
      if ( test == Cont<T>{ a, b } )
        return true;

    return false;
  }

  // partition :: ( T -> bool ) -> [T] -> ( [T], [T] )
  template <typename T, typename F, template <typename> typename Cont>
  auto partition( F predicate, Cont<T> const& xs ) -> Tup<Cont<T>, Cont<T>>
  {
    Cont<T> left{};
    Cont<T> right{};

    for ( auto const& x : xs )
      if ( predicate( x ) )
        left.push_back( x );
      else
        right.push_back( x );

    return std::make_tuple( left, right );
  }

  // nub :: [T] -> [T]
  template <typename T, template <typename> typename Cont>
  auto nub( Cont<T> const& xs ) -> Cont<T>
  {
    Cont<T> out{};

    for ( auto const& x : xs )
      if ( ! elem( x, out ) )
        out.push_back( x );

    return out;
  }
}

#endif