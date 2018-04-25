#ifndef functional_algorithms_header
#define functional_algorithms_header

#include <vector>
#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <optional>

namespace fct
{
  using String = std::string;
  using SS = std::stringstream;
  using Float = float;
  using Double = long double;
  using Int = int;
  using Char = char;

  template <typename T>
  using Opt = std::optional<T>;

  template <typename T>
  using Vec = std::vector<T>;

  // template <typename S, typename T>
  template <typename ...Ts>
  using Tup = std::tuple<Ts...>;

  Double pi = 3.141592653589793238462643383279502884;
  Double e  = 2.718281828459045235360287471352662497;

  template <typename T>
  auto print( Opt<T> const& val, Char end = '\n' ) -> void
  {
    if ( val )
      std::cout << val.value() << end;
    else
      std::cout << "Nothing" << end;
  }

  // print :: T -> void
  template <typename T>
  auto print( T const& val, Char end = '\n' ) -> void
  {
    std::cout << val << end;
  }

  // print :: [T] -> void
  template <typename T>
  auto print( Vec<T> const& xs, Char end = '\n' ) -> void
  {
    if ( xs.empty() )
    {
      std::cout << "[]" << end;
      return;
    }

    std::cout << "[";
    auto i = xs.begin();
    for ( ; i < xs.end() - 1; i++ )
      std::cout << *i << ',';
    std::cout << *i << "]" << end;
  }

  // print :: [[T]] -> void
  template <typename T>
  auto print( Vec<Vec<T>> const& xxs, Char end = '\n' ) -> void
  {
    std::cout << "[ ";
    for ( auto const& xs : xxs )
    {
      std::cout << "[ ";
      for ( auto const& x : xs )
      {
        std::cout << x << ' ';
      }
      std::cout << "] ";
    }
    std::cout << "]" << end;
  }

  // fmap :: ( S -> T ) -> [S] -> [T]
  template <typename S, typename T, typename F>
  auto fmap( F fct, Vec<S> const& xs ) -> Vec<T>
  {
    Vec<T> out{};
    out.reserve( xs.size() );

    for ( auto const& x : xs )
      out.push_back( fct( x ) );

    return out;
  }

  // transpose :: [[T]] -> [[T]]
  template <typename T>
  auto transpose( Vec<Vec<T>> const& xxs ) -> Vec<Vec<T>>
  {
    Vec<Vec<T>> out{};

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

  // filter :: ( T -> bool ) -> [T]
  template <typename T, typename F>
  auto filter( F predicate, Vec<T> const& xs ) -> Vec<T>
  {
    Vec<T> out{};
    out.reserve( xs.size() );

    for ( auto const& x : xs )
      if ( predicate( x ) )
        out.push_back( x );

    return out;
  }

  // takeWhile :: ( T -> bool ) -> [T] -> [T]
  template <typename T, typename F>
  auto takeWhile( F predicate, Vec<T> const& xs ) -> Vec<T>
  {
    Vec<T> out{};
    out.reserve( xs.size() );

    for ( auto const& x : xs )
      if ( predicate( x ) )
        out.push_back( x );
      else
        break;

    return out;
  }

  // dropWhile :: ( T -> bool ) -> [T] -> [T]
  template <typename T, typename F>
  auto dropWhile( F predicate, Vec<T> const& xs ) -> Vec<T>
  {
    Int i = 0;
    for ( auto const& x : xs )
      if ( predicate( x ) )
        i++;
      else
        break;

    return Vec<T>{ xs.begin() + i, xs.end() };
  }

  // head :: [T] -> Opt<T>
  template <typename T>
  auto head( Vec<T> const& xs ) -> Opt<T>
  {
    if ( xs.empty() )
      return Opt<T>{};

    return Opt<T>{ xs.at( 0 ) };
  }

  // tail :: [T] -> [T]
  template <typename T>
  auto tail( Vec<T> const& xs ) -> Vec<T>
  {
    return Vec<T>{ xs.begin() + 1, xs.end() };
  }

  // init :: [T] -> [T]
  template <typename T>
  auto init( Vec<T> const& xs ) -> Vec<T>
  {
    return Vec<T>{ xs.begin(), xs.end() - 1 };
  }

  // last :: [T] -> Opt<T>
  template <typename T>
  auto last( Vec<T> const& xs ) -> Opt<T>
  {
    if ( xs.empty() )
      return Opt<T>{};

    return Opt<T>{ *(xs.end() - 1) };
  }

  // subsets :: [T] -> [[T]]
  template <typename T>
  auto subsets( Vec<T> const& xs ) -> Vec<Vec<T>>
  {
    Vec<Vec<T>> out{};
    out.reserve( std::pow( 2, xs.size() ) );

    // Add the empty set
    out.push_back( Vec<T>{} );

    for ( auto const& x : xs )
    {
      // Create copy of current subsets
      Vec<Vec<T>> new_subsets{ out.begin(), out.end() };

      // Add element to every new subset
      for ( Vec<T>& s : new_subsets )
        s.push_back( x );

      // Add new subsets to output
      out.insert( out.end(), new_subsets.begin(), new_subsets.end() );
    }

    return out;
  }

  // even :: Num -> bool
  template <typename T>
  auto even( T const& val ) -> bool
  {
    return ! ( val & 1 );
  }

  // odd :: Num -> bool
  template <typename T>
  auto odd( T const& val ) -> bool
  {
    return ! even( val );
  }

  // abs :: Num -> Num
  template <typename T>
  auto abs( T const& x ) -> T
  {
    return x < 0 ? -x : x;
  }

  // abs :: Num -> Int
  template <typename T>
  auto signum( T const& x ) -> Int
  {
    if ( x < 0 )
      return -1;
    else if ( x > 0 )
      return 1;

    return 0;
  }

  // quotRem :: (Integral a) => T -> T -> ( a, a )
  template <typename T>
  auto quotRem( T const& x, T const& y ) -> Tup<T, T>
  {
    T q = x / y;
    T r = x % y;

    return std::make_tuple( q, r );
  }

  // exp :: Num -> Double
  template <typename T>
  auto exp( T const& x ) -> Double
  {
    return std::pow( e, x );
  }

  // log :: Num -> Double
  template <typename T>
  auto log( T const& x ) -> Double
  {
    return std::log( x );
  }

  // logBase :: T -> T -> T
  template <typename T>
  auto logBase( T const& base, T const& x ) -> Double
  {
    return std::log( x ) / std::log( base );
  }

  // sqrt :: Num -> Double
  template <typename T>
  auto sqrt( T const& x ) -> Double
  {
    return std::sqrt( x );
  }

  // gcd :: T -> T -> T
  template <typename T>
  auto gcd( T const& x, T const& y ) -> T
  {
    T end = x <= y ? x : y;

    T j = 0;
    for ( T i = 1 ; i <= end; i++ )
      if ( abs( x ) % i == 0 && abs( y ) % i == 0 )
        j = i;

    return j;
  }

  // lcm :: T -> T -> T
  template <typename T>
  auto lcm( T const& x, T const& y ) -> T
  {
    T end = x <= y ? x : y;

    for ( T i = 2 ; i <= end; i++ )
      if ( x % i == 0 && y % i == 0 )
        return i;

    return 1;
  }

  // elem :: T -> [b] -> bool
  template <typename T>
  auto elem( T const& el, Vec<T> const& xs ) -> bool
  {
    return any( [&el]( auto const& x ){ return el == x; }, xs );
  }

  // notElem :: T -> [T] -> bool
  template <typename T>
  auto notElem( T const& el, Vec<T> const& xs ) -> bool
  {
    return ! elem( el, xs );
  }

  // maximum :: [T] -> Opt<T>
  template <typename T>
  auto maximum( Vec<T> const& xs ) -> Opt<T>
  {
    if ( xs.empty() )
      return Opt<T>{};

    T out = xs.at( 0 );

    for ( auto const& x : xs )
      if ( x > out )
        out = x;

    return Opt<T>{ out };
  }

  // minimum :: [T] -> Opt<T>
  template <typename T>
  auto minimum( Vec<T> const& xs ) -> Opt<T>
  {
    if ( xs.empty() )
      return Opt<T>{};

    T out = xs.at( 0 );

    for ( auto const& x : xs )
      if ( x < out )
        out = x;

    return Opt<T>{ out };
  }

  // sum :: [T] -> T
  template <typename T>
  auto sum( Vec<T> const& xs ) -> T
  {
    if ( xs.empty() )
      return 0;

    T out = xs.at( 0 );
    for ( auto i = xs.begin() + 1; i < xs.end(); i++ )
      out += *i;

    return out;
  }

  // product :: [T] -> T
  template <typename T>
  auto product( Vec<T> const& xs ) -> T
  {
    if ( xs.empty() )
      return 1;

    T out = xs.at( 0 );
    for ( auto i = xs.begin() + 1; i < xs.end(); i++ )
      out *= *i;

    return out;
  }

  // id :: T -> T
  template <typename T>
  auto id( T const& x ) -> T
  {
    return x;
  }

  // constant :: S -> T -> S
  template <typename S, typename T>
  auto constant( S const& x, T const& y ) -> S
  {
    // Do some gymnastics to
    // get around unused variable warning
    T out = y;
    out = out;
    return x;
  }

  // flip :: ( S -> T -> U ) -> T -> S -> U
  template <typename S, typename T, typename U, typename F>
  auto flip( F func, S const& x, T const& y ) -> U
  {
    return func( y, x );
  }

  // until :: ( T -> bool ) -> ( T -> T ) -> T -> T
  template <typename T, typename P, typename F>
  auto until( P predicate, F func, T const& x ) -> T
  {
    T out = x;
    while ( ! predicate( out ) )
      out = func( out );

    return out;
  }

  // null :: [T] -> bool
  template <typename T>
  auto null( Vec<T> const& xs ) -> bool
  {
    return xs.empty();
  }

  // length :: [T] -> Num
  template <typename S, typename T = size_t>
  auto length( Vec<S> const& xs ) -> T
  {
    return xs.size();
  }

  // reverse :: [T] -> [T]
  template <typename T>
  auto reverse( Vec<T> const& xs ) -> Vec<T>
  {
    Vec<T> out = xs;
    std::reverse( out.begin(), out.end() );
    return out;
  }

  // conjunction :: [T] -> bool
  template <typename T>
  auto conjunction( Vec<T> const& xs ) -> bool
  {
    if ( xs.empty() )
      return false;

    bool out = xs.at( 0 );
    for ( auto i = xs.begin() + 1; i < xs.end(); i++ )
      out = out && *i;

    return out;
  }

  // disjunction :: [T] -> bool
  template <typename T>
  auto disjunction( Vec<T> const& xs ) -> bool
  {
    if ( xs.empty() )
      return false;

    bool out = xs.at( 0 );
    for ( auto i = xs.begin() + 1; i < xs.end(); i++ )
      out = out || *i;

    return out;
  }

  // any :: ( T -> bool ) -> [T] -> bool
  template <typename T, typename F>
  auto any( F predicate, Vec<T> const& xs ) -> bool
  {
    return std::any_of( xs.begin(), xs.end(), predicate );
  }

  // all :: ( T -> bool ) -> [T] -> bool
  template <typename T, typename F>
  auto all( F predicate, Vec<T> const& xs ) -> bool
  {
    return std::all_of( xs.begin(), xs.end(), predicate );
  }

  // concat :: [[T]] -> [T]
  template <typename T>
  auto concat( Vec<Vec<T>> const& xxs ) -> Vec<T>
  {
    // Count number of items;
    size_t i = 0;
    for ( auto const& xs : xxs )
      i += xs.size();

    Vec<T> out{};
    out.reserve( i );

    for ( auto const& xs : xxs )
      out.insert( out.end(), xs.begin(), xs.end() );

    return out;
  }

  // replicate :: Int -> T - [T]
  template <typename T>
  auto replicate( Int num, T const& val ) -> Vec<T>
  {
    return Vec<T>( num, val );
  }

  // take :: Int -> [T] -> [T]
  template <typename T>
  auto take( Int num, Vec<T> const& xs ) -> Vec<T>
  {
    return Vec<T>{ xs.begin(), xs.begin() + num };
  }

  // drop :: Int -> [T] -> [T]
  template <typename T>
  auto drop( Int num, Vec<T> const& xs ) -> Vec<T>
  {
    return Vec<T>{ xs.begin() + num, xs.end() };
  }

  // splitAt :: Int -> [T] -> ( [T], [T] )
  template <typename T>
  auto splitAt( Int index, Vec<T> const& xs ) -> Tup<Vec<T>, Vec<T>>
  {
    Vec<T> left{ xs.begin(), xs.begin() + index };
    Vec<T> right{ xs.begin() + index, xs.end() };

    return std::make_tuple( left, right );
  }

  // lines :: String -> [String]
  auto lines( String const& str ) -> Vec<String>
  {
    Vec<String> out{};

    SS ss;
    ss.str( str );
    for ( String line; std::getline( ss, line ); )
      out.push_back( line );

    return out;
  }

  // words :: String -> [String]
  auto words( String const& str ) -> Vec<String>
  {
    Vec<String> out{};

    SS ss;
    ss.str( str );
    for ( String line; std::getline( ss, line, ' ' ); )
      out.push_back( line );

    return out;
  }

  // unlines :: [String] -> String
  auto unlines( Vec<String> const& xs ) -> String
  {
    String out = "";
    for ( auto const& x : xs )
      out += x + "\n";

    return out;
  }

  // unwords :: [String] -> String
  auto unwords( Vec<String> const& xs ) -> String
  {
    String out = "";
    for ( auto const& x : xs )
      out += x + " ";

    return out;
  }

  // putChar :: Char -> void
  auto putChar( Char const& ch ) -> void
  {
    std::cout << ch;
  }

  // putStr :: String -> void
  auto putStr( String const& str ) -> void
  {
    std::cout << str;
  }

  // putStrLn :: String -> void
  auto putStrLn( String const& str ) -> void
  {
    std::cout << str << '\n';
  }

  // getChar :: Char
  auto getChar() -> Char
  {
    Char out;
    std::cin >> out;
    return out;
  }

  // getLine :: String
  auto getLine() -> String
  {
    String out = "";
    std::getline( std::cin, out );
    return out;
  }

  // readFile :: String -> String
  auto readFile( String const& filePath ) -> String
  {
    std::ifstream file( filePath, std::ios_base::in );

    return String{ std::istreambuf_iterator<Char>(file), std::istreambuf_iterator<Char>() };
  }

  // readLn :: String -> String
  auto readLn( String const& filePath ) -> String
  {
    std::ifstream file( filePath, std::ios_base::in );
    String out = "";
    std::getline( file, out );
    return out;
  }

  // writeFile :: String -> String -> void
  auto writeFile( String const& filePath, String const& str ) -> void
  {
    std::ofstream file( filePath, std::ios_base::out );
    file << str;
  }

  // appendFile :: String -> String -> void
  auto appendFile( String const& filePath, String const& str ) -> void
  {
    std::ofstream file( filePath, std::ios_base::app );
    file << str;
  }

  // show :: T -> String
  template <typename T>
  auto show( T const& x ) -> String
  {
    return std::to_string( x );
  }

  // intersect :: [T] -> [T] -> [T]
  template <typename T>
  auto intersect( Vec<T> const& xs, Vec<T> const& ys ) -> Vec<T>
  {
    Vec<T> out{};
    Vec<T> const* smaller = xs.size() <= ys.size() ? &xs : &ys;
    Vec<T> const* larger = xs.size() > ys.size() ? &xs : &ys;

    for ( auto const& s : *smaller )
      if ( elem( s, *larger ) )
        out.push_back( s );

    return out;
  }

  // union :: [T] -> [T] -> [T]
  template <typename T>
  auto union_of( Vec<T> const& xs, Vec<T> const& ys ) -> Vec<T>
  {
    Vec<T> out{};
    Vec<T> const* smaller = xs.size() <= ys.size() ? &xs : &ys;
    Vec<T> const* larger = xs.size() > ys.size() ? &xs : &ys;

    for ( auto const& s : *smaller )
      if ( notElem( s, *larger ) )
        out.push_back( s );

    for ( auto const& l : *larger )
      out.push_back( l );

    return out;
  }

  // Intersperse :: T -> [T] -> [T]
  template <typename T>
  auto intersperse( T const& y, Vec<T> const& xs ) -> Vec<T>
  {
    Vec<T> out{};

    auto i = xs.begin();
    for ( ; i < xs.end() - 1; i++ )
    {
      out.push_back( *i );
      out.push_back( y );
    }
    out.push_back( *i );

    return out;
  }

  // Intercalate :: [T] -> [[T]] -> [T]
  template <typename T>
  auto intercalate( Vec<T> const& xs, Vec<Vec<T>> const& xxs ) -> Vec<T>
  {
    return concat( intersperse( xs, xxs ) );
  }

  // permutations :: [T] -> [[T]]
  template <typename T>
  auto permutations( Vec<T> const& xs ) -> Vec<Vec<T>>
  {
    Vec<Vec<T>> out{};
    Vec<T> mut_xs = xs;

    std::sort( mut_xs.begin(), mut_xs.end() );

    do
    {
      out.push_back( mut_xs );
    }
    while ( std::next_permutation( mut_xs.begin(), mut_xs.end() ) );

    return out;
  }

  // iterate :: (T -> T) -> T -> [T]
  template <typename T, typename F>
  auto iterate( Int num, F func, T const& x ) -> Vec<T>
  {
    Vec<T> out{};
    for ( uint i = 0; i < num; i++ )
      out.push_back( func( x ) );

    return out;
  }

  // span :: (T -> Bool) -> [T] -> ([T], [T])
  template <typename T, typename F>
  auto span( F predicate, Vec<T> const& xs ) -> Tup<Vec<T>, Vec<T>>
  {
    Int i = 0;
    for ( auto const& x : xs )
      if ( predicate( x ) )
        i++;
      else
        break;

    return std::make_tuple( Vec<T>{ xs.begin(), xs.begin() + i }, Vec<T>{ xs.begin() + i, xs.end() } );
  }

  // break_of :: (T -> Bool) -> [T] -> ([T], [T])
  template <typename T, typename F>
  auto break_of( F predicate, Vec<T> const& xs ) -> Tup<Vec<T>, Vec<T>>
  {
    Int i = 0;
    for ( auto const& x : xs )
      if ( ! predicate( x ) )
        i++;
      else
        break;

    return std::make_tuple( Vec<T>{ xs.begin(), xs.begin() + i }, Vec<T>{ xs.begin() + i, xs.end() } );
  }

  // group :: [T] -> [[T]]
  template <typename T>
  auto group( Vec<T> const& xs ) -> Vec<Vec<T>>
  {
    Vec<Vec<T>> out{};

    auto a = xs.begin();
    auto b = a;

    while ( b < xs.end() )
    {
      if ( *b == *a )
      {
        b++;
      }
      else
      {
        out.push_back( Vec<T>{ a, b } );
        a = b;
      }
    }
    out.push_back( Vec<T>{ a, b } );

    return out;
  }

  // inits :: [T] -> [[T]]
  template <typename T>
  auto inits( Vec<T> const& xs ) -> Vec<Vec<T>>
  {
    Vec<Vec<T>> out{};

    auto i = xs.begin();

    for ( ; i < xs.end(); i++ )
      out.push_back( Vec<T>{ xs.begin(), i } );
    out.push_back( Vec<T>{ xs.begin(), i } );

    return out;
  }

  // tails :: [T] -> [[T]]
  template <typename T>
  auto tails( Vec<T> const& xs ) -> Vec<Vec<T>>
  {
    Vec<Vec<T>> out{};

    auto i = xs.end();

    for ( ; i < xs.begin(); i-- )
      out.push_back( Vec<T>{ xs.begin(), i } );
    out.push_back( Vec<T>{ xs.begin(), i } );

    return out;
  }

  // isPrefixOf :: [T] -> [T] -> Bool
  template <typename T>
  auto isPrefixOf( Vec<T> const& xs, Vec<T> const& ys ) -> bool
  {
    auto ys_inits = inits( ys );
    return elem( xs, ys_inits );
  }

  // isSuffixOf :: [T] -> [T] -> Bool
  template <typename T>
  auto isSuffixOf( Vec<T> const& xs, Vec<T> const& ys ) -> bool
  {
    auto ys_rev_tails = tails( reverse( ys ) );
    auto xs_rev = reverse( xs );

    return elem( xs_rev, ys_rev_tails );
  }

  // isInfixOf :: [T] -> [T] -> Bool
  template <typename T>
  auto isInfixOf( Vec<T> const& xs, Vec<T> const& ys ) -> bool
  {
    auto a = ys.begin();
    auto b = ys.begin() + xs.size();

    Vec<T> test{ xs.begin(), xs.end() };

    for ( ; b < ys.end(); a++, b++ )
      if ( test == Vec<T>{ a, b } )
        return true;

    return false;
  }

  // partition :: ( T -> bool ) -> [T] -> ( [T], [T] )
  template <typename T, typename F>
  auto partition( F predicate, Vec<T> const& xs ) -> Tup<Vec<T>, Vec<T>>
  {
    Vec<T> left{};
    Vec<T> right{};

    for ( auto const& x : xs )
      if ( predicate( x ) )
        left.push_back( x );
      else
        right.push_back( x );

    return std::make_tuple( left, right );
  }

  // nub :: [T] -> [T]
  template <typename T>
  auto nub( Vec<T> const& xs ) -> Vec<T>
  {
    Vec<T> out{};

    for ( auto const& x : xs )
      if ( ! elem( x, out ) )
        out.push_back( x );

    return out;
  }

  template <typename T>
  auto sort( Vec<T> const& xs ) -> Vec<T>
  {
    Vec<T> out{ xs.begin(), xs.end() };
    std::sort( out.begin(), out.end() );
    return out;
  }
}

#endif
