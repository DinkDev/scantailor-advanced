// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#ifndef SCANTAILOR_IMAGEPROC_SKEWFINDER_H_
#define SCANTAILOR_IMAGEPROC_SKEWFINDER_H_

#include "NonCopyable.h"

namespace imageproc {
class BinaryImage;

/**
 * \brief The result of the "find skew" operation.
 * \see SkewFinder
 */
class Skew {
 public:
  /**
   * \brief The threshold separating good and poor confidence values.
   * \see confidence()
   */
  static const double GOOD_CONFIDENCE;

  Skew() : m_angle(0.0), m_confidence(0.0) {}

  Skew(double angle, double confidence) : m_angle(angle), m_confidence(confidence) {}

  /**
   * \brief Get the skew angle in degrees.
   *
   * Positive values indicate clockwise skews.
   */
  double angle() const { return m_angle; }

  /**
   * \brief Get the confidence value.
   *
   * The worst possible confidence is 0, while everything
   * above or equal to GOOD_CONFIDENCE indicates high
   * confidence level.
   */
  double confidence() const { return m_confidence; }

 private:
  double m_angle;
  double m_confidence;
};


class SkewFinder {
  DECLARE_NON_COPYABLE(SkewFinder)

 public:
  static const double DEFAULT_MAX_ANGLE;

  static const double DEFAULT_MIN_ANGLE;

  static const double DEFAULT_ACCURACY;

  static const int DEFAULT_COARSE_REDUCTION;

  static const int DEFAULT_FINE_REDUCTION;

  SkewFinder();

  /**
   * \brief Set the maximum skew angle, in degrees.
   *
   * The range between 0 and maxAngle degrees both clockwise
   * and counter-clockwise will be checked.
   * \note The angle can't exceed 45 degrees.
   */
  void setMaxAngle(double maxAngle = DEFAULT_MAX_ANGLE);

  /**
   * \brief Set the minimum skew angle, in degrees.
   *
   * Any skew angle less than minAngle degrees will be rejected
   * and zero angle returned.
   * \note The minimum angle can't exceed the maximum skew angle.
   */
  void setMinAngle(double minAngle = DEFAULT_MIN_ANGLE);

  /**
   * \brief Set the desired accuracy.
   *
   * Accuracy is the allowed deviation from the actual skew
   * angle, in degrees.
   */
  void setDesiredAccuracy(double accuracy = DEFAULT_ACCURACY);

  /**
   * \brief Downscale the image before doing a coarse search.
   *
   * Downscaling the image before doing a coarse search will speed
   * things up, but may reduce accuracy.  Specifying a value
   * that is too high will cause totally wrong results.
   * \param reduction The number of times to apply a 2x downscaling
   *                  to the image before doing a coarse search.
   *                  The default value is recommended for 300 dpi
   *                  scans of hight quality material.
   */
  void setCoarseReduction(int reduction = DEFAULT_COARSE_REDUCTION);

  /**
   * \brief Downscale the image before doing a fine search.
   *
   * Downscaling the image before doing a fine search will speed
   * things up, but may reduce accuracy.  Comared to a reduction
   * before a coarse search, it won't give as much of a speed-up,
   * but it won't cause completely wrong results.
   * \param reduction The number of times to apply a 2x downscaling
   *                  to the image before doing a fine search.
   *                  The default value is recommended for 300 dpi
   *                  scans of hight quality material.
   */
  void setFineReduction(int reduction = DEFAULT_FINE_REDUCTION);

  /**
   * \brief Set the horizontal to vertical optical resolution ratio.
   *
   * If horizontal and vertical optical resolutions (DPI) differ,
   * it's necessary to provide their ratio.
   * \param ratio Horizontal optical resolution divided by vertical one.
   */
  void setResolutionRatio(double ratio);

  /**
   * \brief Process the image and determine its skew.
   * \note If the image contains text columns at (slightly) different
   * angles, one of those angles will be found, with a lower confidence.
   */
  Skew findSkew(const BinaryImage& image) const;

 private:
  static const double LOW_SCORE;

  double process(const BinaryImage& src, BinaryImage& dst, double angle) const;

  static double calcScore(const BinaryImage& image);

  double m_maxAngle;
  double m_minAngle;
  double m_accuracy;
  double m_resolutionRatio;
  int m_coarseReduction;
  int m_fineReduction;
};
}  // namespace imageproc
#endif  // ifndef SCANTAILOR_IMAGEPROC_SKEWFINDER_H_
