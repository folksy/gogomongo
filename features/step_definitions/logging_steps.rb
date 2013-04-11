require "mongo"

Given( /^the logging service is running$/ ) do
  run_service
end

When( /^I send (simple|time) events to the service$/ ) do |event_type|
  type = event_type.to_sym
  @events_in = send_events( type )
  sleep( 0.25 )
  @events_out = processed_events( type )
end

Then( /^I should see processed records in the database$/ ) do
  @events_out.size.should == @events_in.size
end

Then( /^the records should have their original miscellaneous keys and values$/ ) do
  [ @events_in, @events_out ].transpose.each do |(ein, eout)|
    ein.keys.each do |key|
      next if key.is_a?( Symbol )
      eout[key].should == ein[key]
    end
  end
end

Then( /^the records should have their original message values$/ ) do
  [ @events_in, @events_out ].transpose.each do |(ein, eout)|
    eout["message"].should == ein["message"]
  end
end
