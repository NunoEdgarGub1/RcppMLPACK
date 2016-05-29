/**
 * @file averge_init.hpp
 * @author Sumedh Ghaisas
 *
 * Initialization rule for Alternating Matrix Factorization.
 */
#ifndef MLPACK_METHODS_AMF_AVERAGE_INIT_HPP
#define MLPACK_METHODS_AMF_AVERAGE_INIT_HPP

#include <mlpack/core.hpp>

namespace mlpack {
namespace amf {

/**
 * This initialization rule initializes matrix W and H to root of the average of
 * V, perturbed with uniform noise. Uniform noise is generated by Armadillo's
 * 'randu' function.  For better performance, the lowest element of the matrix
 * is subtracted from the average before dividing it by the factorization rank.
 * This computed value is added with the random noise.
 */
class AverageInitialization
{
 public:
  // Empty constructor required for the InitializeRule template
  AverageInitialization() { }

  /**
   * Initialize the matrices W and H to the average value of V with uniform
   * random noise added.
   *
   * @param V Input matrix.
   * @param r Rank of matrix.
   * @param W W matrix, to be initialized.
   * @param H H matrix, to be initialized.
   */
  template<typename MatType>
  inline static void Initialize(const MatType& V,
                                const size_t r,
                                arma::mat& W,
                                arma::mat& H)
  {
    const size_t n = V.n_rows;
    const size_t m = V.n_cols;

    double avgV = 0;
    size_t count = 0;
    double min = DBL_MAX;

    // Iterate over all elements in the matrix (for sparse matrices, this only
    // iterates over nonzeros).
    for (typename MatType::const_row_col_iterator it = V.begin();
        it != V.end(); ++it)
    {
      ++count;
      avgV += *it;
      // Track the minimum value.
      if (*it < min)
        min = *it;
    }

    avgV = sqrt(((avgV / (n * m)) - min) / r);

    // Initialize to random values.
    W.randu(n, r);
    H.randu(r, m);

    W = W + avgV;
    H = H + avgV;
  }

  //! Serialize the object (in this case, there is nothing to do).
  template<typename Archive>
  void Serialize(Archive& /* ar */, const unsigned int /* version */) { }
};

} // namespace amf
} // namespace mlpack

#endif