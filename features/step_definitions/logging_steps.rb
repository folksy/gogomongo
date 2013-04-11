require "mongo"

Given( /^the logging service is running$/ ) do
  run_service
end

When( /^I send (.*) events to the service$/ ) do |event_type|
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
      next if key.is_a?( Symbol ) || key == "time"
      eout[key].should == ein[key]
    end
  end
end

Then( /^the records should have their original values, bar (.*)$/ ) do |nfield|
  [ @events_in, @events_out ].transpose.each do |(ein, eout)|
    ein.keys.each do |key|
      next if key.is_a?( Symbol ) || key == "time" || key == nfield
      eout[key].should == ein[key]
    end
  end
end

Then( /^the records should have their original message values$/ ) do
  [ @events_in, @events_out ].transpose.each do |(ein, eout)|
    eout["message"].should == ein["message"]
  end
end

Then( /^the records without an original time value should have a new one$/ ) do
  [ @events_in, @events_out ].transpose.each do |(ein, eout)|
    next if ein["time"]
    eout["time"].should_not be_nil
  end
end

Then( /^the records with original time values should have kept them$/ ) do
  [ @events_in, @events_out ].transpose.each do |(ein, eout)|
    next if ! ein["time"]
    eout["time"].to_i.should == ein["time"].to_i
  end
end
