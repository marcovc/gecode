/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "gecode/set.hh"
#include "examples/support.hh"
#include "gecode/minimodel.hh"

/**
 * \brief %Example: Generating hamming codes
 *
 * Generate a Hamming code that fits in b-bit words to code n symbols where
 * the Hamming distance between every two symbol codes is at least d.
 * The Hamming distance between to words is the number of bit positions
 * where they differ.
 *
 * This instance fixes b to 20 and d to 3.
 *
 * \ingroup ExProblem
 *
 */
class Hamming : public Example {
public:
  /// The hamming code
  SetVarArray xs;

  /// Number of bits
  static const int bits = 20;
  /// Minimum distance between two codes
  static const int dist = 3;

  /// Actual model
  Hamming(const SizeOptions& opt) :
    xs(*this,opt.size(),IntSet::empty,1,bits) {
    SetVarArray cxs(*this,xs.size());
    for (int i=0; i<xs.size(); i++)
      rel(*this, xs[i], SRT_CMPL, cxs[i]);

    for (int i=0; i<xs.size(); i++) {
      SetVar y = xs[i];
      SetVar cy = cxs[i];
      for (int j=i+1; j<xs.size(); j++) {
        SetVar x = xs[j];
        SetVar cx = cxs[j];

        SetVar xIntCy(*this);
        SetVar yIntCx(*this);

        rel(*this, x, SOT_INTER, cy, SRT_EQ, xIntCy);
        rel(*this, y, SOT_INTER, cx, SRT_EQ, yIntCx);
        IntVar diff1(*this,0,1024);
        IntVar diff2(*this,0,1204);
        cardinality(*this, xIntCy,diff1);
        cardinality(*this, yIntCx,diff2);
        post(*this, diff1+diff2 >= dist);
      }
    }

    branch(*this, xs, SET_VAR_NONE, SET_VAL_MIN_INC);
  }

  /// Print solution
  virtual void
  print(std::ostream& os) {
    for (int i=0; i<xs.size(); i++) {
      os << "\t[" << i << "] = " << xs[i] << std::endl;
    }
  }

  /// Constructor for copying \a s
  Hamming(bool share, Hamming& s) : Example(share,s) {
    xs.update(*this, share, s.xs);
  }
  /// Copy during cloning
  virtual Space*
  copy(bool share) {
    return new Hamming(share,*this);
  }

};

/** \brief Main-function
 *  \relates Hamming
 */
int
main(int argc, char* argv[]) {
  SizeOptions opt("Hamming");
  opt.size(32);
  opt.parse(argc,argv);
  Example::run<Hamming,DFS,SizeOptions>(opt);
  return 0;
}


// STATISTICS: example-any
