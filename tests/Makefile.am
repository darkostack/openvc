include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

DIST_SUBDIRS                            = \
    unit                                  \
    $(NULL)

SUBDIRS                                 = \
    $(NULL)

PRETTY_SUBDIRS                          = \
    unit                                  \
    $(NULL)

if OPENVC_POSIX
SUBDIRS                                += \
    unit                                  \
    $(NULL)
endif

if OPENVC_BUILD_TESTS
if OPENVC_BUILD_COVERAGE
CLEANFILES                             	= $(wildcard *.gcda *.gcno)

if OPENVC_BUILD_COVERAGE_REPORTS
# The bundle should positively be qualified with the absolute build
# path. Otherwise, VPATH will get auto-prefixed to it if there is
# already such a directory in the non-colocated source tree.

OPENVC_COVERAGE_BUNDLE             		= ${abs_builddir}/${PACKAGE}${NL_COVERAGE_BUNDLE_SUFFIX}
OPENVC_COVERAGE_INFO               		= ${OPENVC_COVERAGE_BUNDLE}/${PACKAGE}${NL_COVERAGE_INFO_SUFFIX}

$(OPENVC_COVERAGE_BUNDLE):
	$(call create-directory)

$(OPENVC_COVERAGE_INFO): check | $(dir $(OPENVC_COVERAGE_INFO))
	$(call generate-coverage-report,${top_builddir})

coverage-local: $(OPENVC_COVERAGE_INFO)

clean-local: clean-local-coverage

.PHONY: clean-local-coverage
clean-local-coverage:
	-$(AM_V_at)rm -rf $(OPENVC_COVERAGE_BUNDLE)
endif # OPENVC_BUILD_COVERAGE_REPORTS
endif # OPENVC_BUILD_COVERAGE
endif # OPENVC_BUILD_TESTS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
