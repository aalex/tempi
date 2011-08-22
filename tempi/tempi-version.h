#ifndef __MIKADO_VERSION_H__
#define __MIKADO_VERSION_H__

/**
 * MIKADO_MAJOR_VERSION:
 *
 * The major version of the Mikado library (1, if %MIKADO_VERSION is 1.2.3)
 */
#define MIKADO_MAJOR_VERSION   (@MIKADO_MAJOR_VERSION@)

/**
 * MIKADO_MINOR_VERSION:
 *
 * The minor version of the Clutter library (2, if %MIKADO_VERSION is 1.2.3)
 */
#define MIKADO_MINOR_VERSION   (@MIKADO_MINOR_VERSION@)

/**
 * MIKADO_MICRO_VERSION:
 *
 * The micro version of the Clutter library (3, if %MIKADO_VERSION is 1.2.3)
 */
#define MIKADO_MICRO_VERSION   (@MIKADO_MICRO_VERSION@)


/**
 * MIKADO_VERSION:
 *
 * The full version of the Clutter library, like 1.2.3
 */
#define MIKADO_VERSION         @MIKADO_VERSION@

/**
 * MIKADO_VERSION_S:
 *
 * The full version of the Clutter library, in string form (suited for
 * string concatenation)
 */
#define MIKADO_VERSION_S       "@MIKADO_VERSION@"

/**
 * MIKADO_VERSION_HEX:
 *
 * Numerically encoded version of the Clutter library, like 0x010203
 */
#define MIKADO_VERSION_HEX     ((MIKADO_MAJOR_VERSION << 24) | \
                                (MIKADO_MINOR_VERSION << 16) | \
                                (MIKADO_MICRO_VERSION << 8))
/**
 * MIKADO_CHECK_VERSION:
 * @major: major version, like 1 in 1.2.3
 * @minor: minor version, like 2 in 1.2.3
 * @micro: micro version, like 3 in 1.2.3
 *
 * Evaluates to %TRUE if the version of the Mikado library is greater
 * than @major, @minor and @micro
 */
#define MIKADO_CHECK_VERSION(major,minor,micro) \
        (MIKADO_MAJOR_VERSION > (major) || \
        (MIKADO_MAJOR_VERSION == (major) && MIKADO_MINOR_VERSION > (minor)) || \
        (MIKADO_MAJOR_VERSION == (major) && MIKADO_MINOR_VERSION == (minor) && MIKADO_MICRO_VERSION >= (micro)))

#endif // __MIKADO_VERSION_H__

