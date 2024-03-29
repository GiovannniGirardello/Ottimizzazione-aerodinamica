
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE_1_0.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#include <catch2/reporters/catch_reporter_listening.hpp>

#include <catch2/catch_config.hpp>
#include <catch2/internal/catch_move_and_forward.hpp>
#include <catch2/internal/catch_stream.hpp>

#include <cassert>

namespace Catch {
    void ListeningReporter::updatePreferences(IStreamingReporter const& reporterish) {
        m_preferences.shouldRedirectStdOut |=
            reporterish.getPreferences().shouldRedirectStdOut;
        m_preferences.shouldReportAllAssertions |=
            reporterish.getPreferences().shouldReportAllAssertions;
    }

    void ListeningReporter::addListener( IStreamingReporterPtr&& listener ) {
        updatePreferences(*listener);
        m_reporterLikes.insert(m_reporterLikes.begin() + m_insertedListeners, CATCH_MOVE(listener) );
        ++m_insertedListeners;
    }

    void ListeningReporter::addReporter( IStreamingReporterPtr&& reporter ) {
        updatePreferences(*reporter);

        // We will need to output the captured stdout if there are reporters
        // that do not want it captured.
        // We do not consider listeners, because it is generally assumed that
        // listeners are output-transparent, even though they can ask for stdout
        // capture to do something with it.
        m_haveNoncapturingReporters |= !reporter->getPreferences().shouldRedirectStdOut;

        // Reporters can always be placed to the back without breaking the
        // reporting order
        m_reporterLikes.push_back( CATCH_MOVE( reporter ) );
    }

    void ListeningReporter::noMatchingTestCases( StringRef unmatchedSpec ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->noMatchingTestCases( unmatchedSpec );
        }
    }

    void ListeningReporter::fatalErrorEncountered( StringRef error ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->fatalErrorEncountered( error );
        }
    }

    void ListeningReporter::reportInvalidTestSpec( StringRef arg ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->reportInvalidTestSpec( arg );
        }
    }

    void ListeningReporter::benchmarkPreparing( StringRef name ) {
        for (auto& reporterish : m_reporterLikes) {
            reporterish->benchmarkPreparing(name);
        }
    }
    void ListeningReporter::benchmarkStarting( BenchmarkInfo const& benchmarkInfo ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->benchmarkStarting( benchmarkInfo );
        }
    }
    void ListeningReporter::benchmarkEnded( BenchmarkStats<> const& benchmarkStats ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->benchmarkEnded( benchmarkStats );
        }
    }

    void ListeningReporter::benchmarkFailed( StringRef error ) {
        for (auto& reporterish : m_reporterLikes) {
            reporterish->benchmarkFailed(error);
        }
    }

    void ListeningReporter::testRunStarting( TestRunInfo const& testRunInfo ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->testRunStarting( testRunInfo );
        }
    }

    void ListeningReporter::testCaseStarting( TestCaseInfo const& testInfo ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->testCaseStarting( testInfo );
        }
    }

    void
    ListeningReporter::testCasePartialStarting( TestCaseInfo const& testInfo,
                                                uint64_t partNumber ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->testCasePartialStarting( testInfo, partNumber );
        }
    }

    void ListeningReporter::sectionStarting( SectionInfo const& sectionInfo ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->sectionStarting( sectionInfo );
        }
    }

    void ListeningReporter::assertionStarting( AssertionInfo const& assertionInfo ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->assertionStarting( assertionInfo );
        }
    }

    // The return value indicates if the messages buffer should be cleared:
    void ListeningReporter::assertionEnded( AssertionStats const& assertionStats ) {
        const bool reportByDefault =
            assertionStats.assertionResult.getResultType() != ResultWas::Ok ||
            m_config->includeSuccessfulResults();

        for ( auto & reporterish : m_reporterLikes ) {
            if ( reportByDefault ||
                 reporterish->getPreferences().shouldReportAllAssertions ) {
                    reporterish->assertionEnded( assertionStats );
            }
        }
    }

    void ListeningReporter::sectionEnded( SectionStats const& sectionStats ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->sectionEnded( sectionStats );
        }
    }

    void ListeningReporter::testCasePartialEnded( TestCaseStats const& testStats,
                                                  uint64_t partNumber ) {
        if ( m_preferences.shouldRedirectStdOut &&
             m_haveNoncapturingReporters ) {
            if ( !testStats.stdOut.empty() ) {
                Catch::cout() << testStats.stdOut << std::flush;
            }
            if ( !testStats.stdErr.empty() ) {
                Catch::cerr() << testStats.stdErr << std::flush;
            }
        }

        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->testCasePartialEnded( testStats, partNumber );
        }
    }

    void ListeningReporter::testCaseEnded( TestCaseStats const& testCaseStats ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->testCaseEnded( testCaseStats );
        }
    }

    void ListeningReporter::testRunEnded( TestRunStats const& testRunStats ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->testRunEnded( testRunStats );
        }
    }


    void ListeningReporter::skipTest( TestCaseInfo const& testInfo ) {
        for ( auto& reporterish : m_reporterLikes ) {
            reporterish->skipTest( testInfo );
        }
    }

    void ListeningReporter::listReporters(std::vector<ReporterDescription> const& descriptions) {
        for (auto& reporterish : m_reporterLikes) {
            reporterish->listReporters(descriptions);
        }
    }

    void ListeningReporter::listTests(std::vector<TestCaseHandle> const& tests) {
        for (auto& reporterish : m_reporterLikes) {
            reporterish->listTests(tests);
        }
    }

    void ListeningReporter::listTags(std::vector<TagInfo> const& tags) {
        for (auto& reporterish : m_reporterLikes) {
            reporterish->listTags(tags);
        }
    }

} // end namespace Catch
