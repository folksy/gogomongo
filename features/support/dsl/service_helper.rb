module ServiceHelper

  class Service
    attr_reader :process
    
    def initialize
      cmd = "bin/gogomongo start" +
        " --ns #{ CONFIG[:ns] }" +
        " --config #{ CONFIG[:file] }" +
        " --pidfile #{ CONFIG[:pidfile] }" +
        " --errfile #{ CONFIG[:errfile] }"
      puts cmd
      @process = IO.popen( cmd )
    end

    class << self
      def run
        if @_service
          raise( "Service #{ @_service.inspect } already running." )
        else
          @_service = new
          @_service
        end
      end

      def get
        @_service || raise( "No service running" )
      end

      def stop
        if @_service
          puts "bin/gogomongo stop --pidfile #{ CONFIG[:pidfile] }"
          puts `bin/gogomongo stop --pidfile #{ CONFIG[:pidfile] }`
          @_service.process.close
          @_service = nil
        end
      end
    end
  end

  def run_service
    Service.run
  end

  def stop_service
    Service.stop
  end

  def service
    Service.get
  end
end

World( ServiceHelper )

After do
  stop_service
end
