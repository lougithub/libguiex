namespace guiex
{
	class CGUIProperty
	{
		CGUIProperty(const CGUIString& rName=CGUIString(), const CGUIString& rType=CGUIString(), const CGUIString& rValue=CGUIString() );
		CGUIProperty(const CGUIProperty& other );
		~CGUIProperty();

		void Clear();

		void SetName(const CGUIString& rName);
		const CGUIString& GetName() const;

		void SetValue(const CGUIString& rValue);
		const CGUIString& GetValue() const;

		void SetType(const CGUIString& rType);
		uint32 GetType() const;
		const CGUIString& GetTypeAsString() const;

		void AddProperty(  const CGUIProperty& rProperty );
		void RemoveProperty( int32 nIdx );
		void RemoveProperty( const CGUIProperty& rProperty );
		void InsertProperty(  const CGUIProperty& rProperty, int32 nIndex );
		uint32 GetPropertyNum( ) const;
		int32 GetPropertyIndex( const CGUIString& rName ) const;
		const CGUIProperty*	GetProperty( uint32 nIdx ) const;
	};
}

 