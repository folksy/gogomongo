Feature: Logging
  I want to be able to send log entries to the service
  And see them processed to structured records
  And be able to configure further smart processing on different kinds of records
@wip
  @log @simple
  Scenario: Logging simple events
    Given the logging service is running
    When I send simple events to the service
    Then I should see processed records in the database
    And the records should have their original miscellaneous keys and values
    And the records should have their original message values

  @log @time
  Scenario: Logging events with a time processor
    Given Pending

  @log @ruby @object @safe
  Scenario: Logging events with a ruby-object-safe processor
    Given Pending

  @log @unlabelled @hashes
  Scenario: Logging events with a hashes processor
    Given Pending

  @log @labelled @hashes
  Scenario: Logging events with a labelled hashes processor
    Given Pending

  @log @preserve_original_message
  Scenario: Logging events with a preserve original message processor
    Given Pending
