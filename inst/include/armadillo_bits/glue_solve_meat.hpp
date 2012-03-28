// Copyright (C) 2009-2012 NICTA (www.nicta.com.au)
// Copyright (C) 2009-2012 Conrad Sanderson
// 
// This file is part of the Armadillo C++ library.
// It is provided without any warranty of fitness
// for any purpose. You can redistribute this file
// and/or modify it under the terms of the GNU
// Lesser General Public License (LGPL) as published
// by the Free Software Foundation, either version 3
// of the License or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)


//! \addtogroup glue_solve
//! @{



template<typename eT>
inline
void
glue_solve::solve_direct(Mat<eT>& out, Mat<eT>& A, const Mat<eT>& B, const bool slow)
  {
  arma_extra_debug_sigprint();
  
  const uword A_n_rows = A.n_rows;
  const uword A_n_cols = A.n_cols;
  
  arma_debug_check( (A_n_rows != B.n_rows), "solve(): number of rows in A and B must be the same" );
  
  bool status = false;
  
  if(A_n_rows == A_n_cols)
    {
    status = auxlib::solve(out, A, B, slow);
    }
  else
  if(A_n_rows > A_n_cols)
    {
    arma_extra_debug_print("solve(): detected over-determined system");
    status = auxlib::solve_od(out, A, B);
    }
  else
    {
    arma_extra_debug_print("solve(): detected under-determined system");
    status = auxlib::solve_ud(out, A, B);
    }
  
  if(status == false)
    {
    out.reset();
    arma_bad("solve(): solution not found");
    }
  }



template<typename T1, typename T2>
inline
void
glue_solve::apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_solve>& X)
  {
  arma_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  Mat<eT> A = X.A.get_ref();
  
  const unwrap_check<T2> B_tmp(X.B, out);
  const Mat<eT>& B = B_tmp.M;
  
  glue_solve::solve_direct( out, A, B, (X.aux_uword == 1) );
  }



template<typename T1, typename T2>
inline
void
glue_solve_tr::apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_solve_tr>& X)
  {
  arma_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const unwrap_check<T1> A_tmp(X.A, out);
  const unwrap_check<T2> B_tmp(X.B, out);
  
  const Mat<eT>& A = A_tmp.M;
  const Mat<eT>& B = B_tmp.M;
  
  bool  err_state = false;
  char* err_msg   = 0;
  
  arma_debug_set_error( err_state, err_msg, ((&A) == (&B)),           "solve(): A is an alias of B" );
  arma_debug_set_error( err_state, err_msg, (A.n_rows != B.n_rows),   "solve(): number of rows in A and B must be the same" );
  arma_debug_set_error( err_state, err_msg, (A.is_square() == false), "solve(): A is not a square matrix" );
  
  arma_debug_check(err_state, err_msg);
  
  const bool status = auxlib::solve_tr(out, A, B, X.aux_uword);
  
  if(status == false)
    {
    out.reset();
    arma_bad("solve(): solution not found");
    }
  }



//! @}
